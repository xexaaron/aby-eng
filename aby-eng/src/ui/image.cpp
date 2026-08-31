#include "ui/image.hpp"

#include "core/renderer.hpp"

namespace aby::eng::ui {

	Image::Image(const Transform2D& transform, const Material2D& material) :
	    Element(transform),
	    m_Material(material),
	    m_Path("") {
	}

	Image::Image(const Transform2D& transform, const std::filesystem::path& rel_path) :
	    Element(transform),
	    m_Path(rel_path) {
	}

	auto Image::create(const Transform2D& transform, const Material2D& material) -> ref<Image> {
		return std::make_shared<Image>(transform, material);
	}

	auto Image::create(const Transform2D& transform, const std::filesystem::path& rel_path) -> ref<Image> {
		return std::make_shared<Image>(transform, rel_path);
	}

	auto Image::set_tint(const glm::fvec4& tint) -> void {
		m_Material.color = tint;
	}

	auto Image::set_color(const glm::fvec4& color) -> void {
		m_Material.color   = color;
		m_Material.texture = 0;
	}

	auto Image::on_create() -> void {
		Element::on_create();
		if (!m_Path.empty()) {
			m_Texture          = rhi::Texture::create(m_Path);
			m_Material.texture = m_Texture->id();
		}
	}

	auto Image::on_render() -> void {
		auto rect = resolved_layout();
		Renderer2D::quad(Transform2D(rect.pos, rect.size), m_Material);
	}

} // namespace aby::eng::ui
