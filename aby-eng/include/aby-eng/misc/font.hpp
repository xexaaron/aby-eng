#pragma once
#include "common-enums.hpp"
#include "common.hpp"
#include "resource.hpp"
#include "texture.hpp"

#include <filesystem>
#include <glm/glm.hpp>
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

	class Font {
	public:
		/**
        * @brief Font creation function
        * @param rel_path Path relative to cwd/system font folder
        * @param system Is the font contained by the system
        */
		static auto create(const fs::path& rel_path, float px_size = 12.f, bool system = false) -> FontPtr;

		auto path() const -> const fs::path&;
		auto texture() -> rhi::TexturePtr;
		auto pixel_size() const -> float;
		auto glyphs() const -> const std::unordered_map<char32_t, Glyph>&;
		auto glyph(char32_t c) const -> const Glyph&;

		auto is_system() const -> bool;
	protected:
		Font(const fs::path& rel_path, bool system, float px_size, const std::unordered_map<char32_t, Glyph>& glyphs, rhi::TexturePtr texture);
	private:
		rhi::TexturePtr m_Texture;
		bool bSystem;
		float m_PxSize;
		fs::path m_Path;
		std::unordered_map<char32_t, Glyph> m_Glyphs;
	};

} // namespace aby::eng
