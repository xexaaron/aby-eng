#include "misc/font.hpp"

#include "common-enums.hpp"
#include "log.hpp"
#include "misc/utf8.hpp"

#include <aby-rhi/context.hpp>
#include <aby-rhi/resource.hpp>
#include <aby-rhi/texture.hpp>
#include <algorithm>
#include <freetype/freetype.h>
#include <freetype2/ft2build.h>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace aby::eng {

	static rhi::ResourceContainer<Font, FONT_RESOURCE_TYPE> s_Fonts;

	namespace {

		FT_Library s_FreeType = nullptr;
		std::once_flag s_FreeTypeInit;

		auto get_freetype() -> FT_Library {
			std::call_once(s_FreeTypeInit, [] {
				FT_Error err = FT_Init_FreeType(&s_FreeType);

				if (err != 0) {
					log_err(
					    "failed to initialize freetype library: {}",
					    FT_Error_String(err));

					s_FreeType = nullptr;
				}
			});

			return s_FreeType;
		}

		struct LoadedGlyph {
			char32_t codepoint;
			FT_GlyphSlot glyph;
		};

	} // namespace

	auto Font::create(const fs::path& rel_path, float px_size, bool system) -> FontPtr {
		auto& ctx  = rhi::Context::get();
		auto* jobs = ctx.job_sys();

		auto resource = s_Fonts.reserve();

		jobs->add_job(rhi::EJobPriority::medium, [resource, rel_path, px_size, system]() {
			FT_Library library = get_freetype();

			if (!library) {
				s_Fonts.fail(resource);
				return;
			}

			fs::path abs_path = rel_path;

			if (system) {
#ifdef __linux__
				if (!rel_path.is_absolute()) {
					abs_path = fs::path("/usr/share/fonts/TTF") / rel_path;
				}
#elif defined(_WIN32)
				if (!rel_path.is_absolute()) {
					abs_path =
					    fs::path("C:/Windows/Fonts") / rel_path;
				}
#elif defined(__APPLE__)
				if (!rel_path.is_absolute()) {
					abs_path =
					    fs::path("/System/Library/Fonts") / rel_path;
				}
#endif
			} else {
				abs_path = rhi::Context::get().file_io()->path(rel_path);
			}

			FT_Face face = nullptr;

			if (FT_Error err = FT_New_Face(library, abs_path.string().c_str(), 0, &face); err != 0) {
				log_err("failed to load freetype face '{}': {}", abs_path.string(), FT_Error_String(err));
				s_Fonts.fail(resource);
				return;
			}

			struct FaceGuard {
				FT_Face face;

				~FaceGuard() {
					if (face) {
						FT_Done_Face(face);
					}
				}
			} face_guard{ face };

			if (FT_Error err = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(px_size)); err != 0) {
				log_err("failed to set font size for '{}': {}", abs_path.string(), FT_Error_String(err));
				s_Fonts.fail(resource);
				return;
			}

			std::unordered_map<char32_t, Glyph> glyphs;

			constexpr char32_t first       = 32;
			constexpr char32_t last        = 126;
			constexpr uint32_t padding     = 1;
			constexpr uint32_t atlas_width = 1024;
			constexpr uint8_t channels     = 4;

			uint32_t atlas_x      = padding;
			uint32_t atlas_y      = padding;
			uint32_t row_height   = 0;
			uint32_t atlas_height = 0;
			FontData font_data    = {};

			font_data.mono = FT_IS_FIXED_WIDTH(face);

			struct Bitmap {
				char32_t codepoint;
				std::vector<uint8_t> pixels;
				uint32_t width;
				uint32_t height;
				int bearing_x;
				int bearing_y;
				float advance;
				uint32_t x;
				uint32_t y;
			};

			std::vector<Bitmap> bitmaps;
			bitmaps.reserve(last - first + 1);

			for (char32_t codepoint = first; codepoint <= last; ++codepoint) {
				FT_UInt glyph_index = FT_Get_Char_Index(face, codepoint);

				if (glyph_index == 0) {
					continue;
				}

				constexpr FT_Int32 load_flags = FT_LOAD_RENDER | FT_LOAD_COLOR;

				if (FT_Error err = FT_Load_Glyph(face, glyph_index, load_flags); err != 0) {
					log_wrn("failed to load glyph U+{:04X}: {}", static_cast<uint32_t>(codepoint), FT_Error_String(err));
					continue;
				}

				auto& bitmap = face->glyph->bitmap;

				uint32_t width  = bitmap.width;
				uint32_t height = bitmap.rows;

				/*
                     * Wrap to the next row if this glyph doesn't fit.
                     */
				if (atlas_x + width + padding > atlas_width) {
					atlas_x     = padding;
					atlas_y    += row_height + padding;
					row_height  = 0;
				}

				Bitmap out{
					.codepoint = codepoint,
					.pixels    = {},
					.width     = width,
					.height    = height,
					.bearing_x = face->glyph->bitmap_left,
					.bearing_y = face->glyph->bitmap_top,
					.advance =
					    static_cast<float>(
					        face->glyph->advance.x) /
					    64.0f,
					.x = atlas_x,
					.y = atlas_y,
				};

				out.pixels.resize(static_cast<size_t>(width) * height * 4);
				auto* dst = reinterpret_cast<uint8_t*>(out.pixels.data());

				for (uint32_t y = 0; y < height; ++y) {
					const auto* src = bitmap.buffer + static_cast<size_t>(y) * std::abs(bitmap.pitch);

					for (uint32_t x = 0; x < width; ++x) {
						uint8_t r = 255;
						uint8_t g = 255;
						uint8_t b = 255;
						uint8_t a = 255;

						switch (bitmap.pixel_mode) {
							case FT_PIXEL_MODE_GRAY: {
								const uint8_t coverage = src[x];
								r                      = 255;
								g                      = 255;
								b                      = 255;
								a                      = coverage;
								break;
							}
							case FT_PIXEL_MODE_BGRA: {
								const auto* pixel = src + x * 4;
								b                 = pixel[0];
								g                 = pixel[1];
								r                 = pixel[2];
								a                 = pixel[3];
								break;
							}
							default:
								log_wrn("unsupported FreeType pixel mode {} for glyph U+{:04X}",
								        static_cast<int>(bitmap.pixel_mode),
								        static_cast<uint32_t>(codepoint));

								r = 255;
								g = 255;
								b = 255;
								a = 0;
								break;
						}

						const size_t index = (static_cast<size_t>(y) * width + x) * 4;
						dst[index + 0]     = r;
						dst[index + 1]     = g;
						dst[index + 2]     = b;
						dst[index + 3]     = a;
					}
				}

				bitmaps.emplace_back(std::move(out));

				atlas_x    += width + padding;
				row_height  = std::max(row_height, height);

				atlas_height = std::max(atlas_height, atlas_y + row_height + padding);
			}

			std::vector<uint8_t> atlas(static_cast<size_t>(atlas_width) * atlas_height * channels);

			for (auto& bitmap : bitmaps) {
				for (uint32_t y = 0; y < bitmap.height; ++y) {
					auto* dst = atlas.data() + (static_cast<size_t>(bitmap.y + y) * atlas_width + bitmap.x) * channels;
					auto* src = reinterpret_cast<const uint8_t*>(bitmap.pixels.data()) + static_cast<size_t>(y) * bitmap.width * channels;
					std::memcpy(dst, src, static_cast<size_t>(bitmap.width) * channels);
				}

				Glyph glyph{
					.uv_min = {
					           static_cast<float>(bitmap.x) / atlas_width,
					           static_cast<float>(bitmap.y) / atlas_height                                                                            },
					.uv_max  = { static_cast<float>(bitmap.x + bitmap.width) / atlas_width, static_cast<float>(bitmap.y + bitmap.height) / atlas_height },
					.size    = {                          static_cast<float>(bitmap.width),                           static_cast<float>(bitmap.height) },
					.bearing = {                      static_cast<float>(bitmap.bearing_x),                        static_cast<float>(bitmap.bearing_y) },
					.advance = bitmap.advance
				};

				glyphs.emplace(bitmap.codepoint, glyph);
			}

			if (font_data.mono) {
				font_data.mono_advance = bitmaps[0].advance;
			}
			font_data.line_height = face->size->metrics.height / 64.f;
			font_data.pixel_size  = px_size;
			font_data.system      = system;

			size_t non_zero_alpha = 0;

			for (size_t i = 3; i < atlas.size(); i += 4) {
				if (atlas[i] != 0) {
					++non_zero_alpha;
				}
			}

			auto texture = rhi::Texture::create(atlas_width, atlas_height, channels, std::move(atlas));

			s_Fonts.add(resource, new Font(rel_path, font_data, glyphs, texture));
		});

		return rhi::create_resource(resource, s_Fonts);
	}

	Font::Font(const fs::path& rel_path, const FontData& data, const std::unordered_map<char32_t, Glyph>& glyphs, rhi::TexturePtr texture) :
	    m_Path(rel_path),
	    m_Data(data),
	    m_Glyphs(glyphs),
	    m_Texture(texture) {
	}

	auto Font::measure(std::string_view text) -> glm::fvec2 {
		if (text.empty()) {
			return { 0.0f, 0.0f };
		}

		float width     = 0.0f;
		float max_width = 0.0f;
		size_t lines    = 1;

		for (char32_t cp : utf8::codepoints(text)) {
			if (cp == U'\n') {
				max_width = std::max(max_width, width);
				width     = 0.0f;
				++lines;
				continue;
			}

			if (m_Data.mono) {
				width += m_Data.mono_advance;
			} else {
				width += glyph(cp).advance;
			}
		}

		max_width = std::max(max_width, width);

		return {
			max_width,
			static_cast<float>(lines) * m_Data.line_height
		};
	}

	auto Font::measure_height(std::string_view text) -> float {
		if (text.empty()) {
			return 0.0f;
		}

		size_t lines = 1;

		for (char32_t cp : utf8::codepoints(text)) {
			if (cp == U'\n') {
				++lines;
			}
		}

		return static_cast<float>(lines) * m_Data.line_height;
	}

	auto Font::measure_width(std::string_view text) -> float {
		if (text.empty()) {
			return 0.f;
		}

		if (m_Data.mono) {
			auto ct = static_cast<float>(utf8::codepoints(text).size());
			return ct * m_Data.mono_advance;
		}

		float width = 0.0f;

		for (char32_t cp : utf8::codepoints(text)) {
			const Glyph& g  = glyph(cp);
			width          += g.advance;
		}

		return width;
	}

	auto Font::path() const -> const fs::path& {
		return m_Path;
	}

	auto Font::texture() -> rhi::TexturePtr {
		return m_Texture;
	}

	auto Font::pixel_size() const -> float {
		return m_Data.pixel_size;
	}

	auto Font::line_height() const -> float {
		return m_Data.line_height;
	}

	auto Font::glyphs() const -> const std::unordered_map<char32_t, Glyph>& {
		return m_Glyphs;
	}

	auto Font::glyph(char32_t c) const -> const Glyph& {
		return m_Glyphs.at(c);
	}

	auto Font::is_system() const -> bool {
		return m_Data.system;
	}

	auto Font::is_mono() const -> bool {
		return m_Data.mono;
	}

} // namespace aby::eng
