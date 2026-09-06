#pragma once
#include "common-enums.hpp"
#include "common.hpp"
#include "misc/utf8.hpp"
#include "resource.hpp"
#include "texture.hpp"

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace aby::eng {

	namespace fs = std::filesystem;

	static constexpr rhi::EResource FONT_RESOURCE_TYPE =
	    static_cast<rhi::EResource>(
	        std::to_underlying(rhi::EResource::user) +
	        1);

	class Font;

	using FontPtr = rhi::ResourcePtr<Font, FONT_RESOURCE_TYPE>;

	struct Glyph {
		glm::fvec2 uv_min;
		glm::fvec2 uv_max;
		glm::fvec2 size;
		glm::fvec2 bearing;
		float advance;
	};

	struct FontData {
		bool system        = false;
		bool mono          = false;
		float pixel_size   = 0.f;
		float line_height  = 0.f;
		float mono_advance = 0.f;
	};

	class Font {
	public:
		/**
        * @brief Font creation function
        * @param rel_path Path relative to cwd/system font folder
        * @param system Is the font contained by the system
		* @note system fonts: 
		* 		linux:/usr/share/fonts/TTF,
		* 		win32:C:/Windows/Fonts,
		*  		macos:/System/Library/Fonts
        */
		static auto create(const fs::path& rel_path, float px_size = 12.f, bool system = false) -> FontPtr;
		~Font();

		auto path() const -> const fs::path&;
		auto texture() -> rhi::TexturePtr;
		auto pixel_size() const -> float;
		auto line_height() const -> float;
		auto glyphs() const -> const std::unordered_map<utf8::codepoint, Glyph>&;
		auto glyph(utf8::codepoint c) const -> const Glyph&;
		auto measure(std::string_view text) const -> glm::fvec2;
		auto measure_height(std::string_view text) const -> float;
		auto measure_width(std::string_view text) const -> float;

		auto is_system() const -> bool;
		auto is_mono() const -> bool;
	protected:
		Font(const fs::path& rel_path, const FontData& data, const std::unordered_map<utf8::codepoint, Glyph>& glyphs, rhi::TexturePtr texture);
	private:
		rhi::TexturePtr m_Texture;
		FontData m_Data;
		fs::path m_Path;
		std::unordered_map<utf8::codepoint, Glyph> m_Glyphs;
	};

} // namespace aby::eng
