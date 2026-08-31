#pragma once
#include "ui/element.hpp"

namespace aby::eng::ui {

	/// @brief Image element class
	class ABY_API Image : public Element {
	public:
		/**
		 * @brief Image constructor
		 * @param transform The element transform
		 * @param material The tint/color & image
		 */
		Image(const Transform2D& transform, const Material2D& material);
		/**
		 * @brief Image constructor
		 * @param transform The element transform
		 * @param rel_path Path relative to the CWD to load a texture from
		 */
		Image(const Transform2D& transform, const std::filesystem::path& rel_path);
		/**
		 * @brief Image creation function
		 * @param transform The element transform
		 * @param material The tint/color & image
		 * @return ref<Image>
		 */
		static auto create(const Transform2D& transform, const Material2D& material) -> ref<Image>;
		/**
		 * @brief Image creation function
		 * @param transform The element transform
		 * @param rel_path Path relative to the CWD to load a texture from
		 * @return ref<Image>
		 */
		static auto create(const Transform2D& transform, const std::filesystem::path& rel_path) -> ref<Image>;

		/**
		 * @brief Set the texture tint
		 * @param tint The tint color
		 */
		auto set_tint(const glm::fvec4& tint) -> void;
		/**
		 * @brief Set the image color. Clears the exisiting texture if one is present
		 * @param color The image color
		 */
		auto set_color(const glm::fvec4& color) -> void;

		auto on_create() -> void override;
		auto on_render() -> void override;
	private:
		Material2D m_Material;
		std::filesystem::path m_Path;
		rhi::TexturePtr m_Texture;
	};

} // namespace aby::eng::ui
