#include "ui/containers/container.hpp"

#include "core/renderer.hpp"

namespace aby::eng::ui {

	Container::Container(Transform2D transform, float padding, const Border& border, EStretch stretch) :
	    Element(transform),
	    m_Padding(padding),
	    m_Border(border),
	    m_Stretch(stretch) {
	}

	auto Container::on_tick(const Time& deltatime) -> void {
		Rect2D rect = available_rect();

		for (auto& child : this->children()) {
			const auto& child_size = child->transform().size;

			if (child_size.x <= 0.f || child_size.y <= 0.f) {
				child->set_resolved_layout(Rect2D(rect.pos, { 0.f, 0.f }));
				continue;
			}

			switch (m_Stretch) {
				case EStretch::fill: {
					child->set_resolved_layout(rect);
					break;
				}
				case EStretch::fit: {
					const float scale     = std::min(rect.size.x / child_size.x, rect.size.y / child_size.y);
					const glm::fvec2 size = child_size * scale;
					const glm::fvec2 pos  = {
						rect.pos.x + (rect.size.x - size.x) * 0.5f,
						rect.pos.y + (rect.size.y - size.y) * 0.5f
					};
					child->set_resolved_layout(Rect2D(pos, size));
					break;
				}
				case EStretch::cover: {
					const float scale     = std::max(rect.size.x / child_size.x, rect.size.y / child_size.y);
					const glm::fvec2 size = child_size * scale;
					const glm::fvec2 pos  = {
						rect.pos.x + (rect.size.x - size.x) * 0.5f,
						rect.pos.y + (rect.size.y - size.y) * 0.5f
					};
					child->set_resolved_layout(Rect2D(pos, size));
					break;
				}
			}
		}

		Element::on_tick(deltatime);
	}

	auto Container::on_render() -> void {
		const auto& pos  = m_Transform.pos;
		const auto& size = m_Transform.size;

		Material2D mat(m_Border.color);

		if (m_Border.top > 0.f) {
			const float half = m_Border.top / 2.f;

			Transform2D transform(
			    { pos.x, pos.y - half },
			    { size.x, m_Border.top },
			    1.f);

			Renderer2D::quad(transform, mat);
		}

		if (m_Border.right > 0.f) {
			const float half = m_Border.right / 2.f;

			Transform2D transform(
			    { pos.x + size.x - half, pos.y },
			    { m_Border.right, size.y },
			    1.f);

			Renderer2D::quad(transform, mat);
		}

		if (m_Border.bottom > 0.f) {
			const float half = m_Border.bottom / 2.f;

			Transform2D transform(
			    { pos.x, pos.y + size.y - half },
			    { size.x, m_Border.bottom },
			    1.f);

			Renderer2D::quad(transform, mat);
		}

		if (m_Border.left > 0.f) {
			const float half = m_Border.left / 2.f;

			Transform2D transform(
			    { pos.x - half, pos.y },
			    { m_Border.left, size.y },
			    1.f);

			Renderer2D::quad(transform, mat);
		}

		Element::on_render();
	}

	auto Container::set_stretch(EStretch stretch) -> void {
		m_Stretch = stretch;
	}

	auto Container::set_border(const Border& border) -> void {
		m_Border = border;
	}

	auto Container::set_border(float scalar, const glm::fvec4& color) -> void {
		m_Border.set_scalar(scalar);
		m_Border.color = color;
	}

	auto Container::set_border(float top, float right, float bottom, float left, const glm::fvec4& color) -> void {
		m_Border.top    = top;
		m_Border.right  = right;
		m_Border.bottom = bottom;
		m_Border.left   = left;
		m_Border.color  = color;
	}

	auto Container::set_padding(float padding) -> void {
		m_Padding = padding;
	}

	auto Container::available_rect() const -> Rect2D {
		auto pos  = m_Transform.pos + glm::fvec2(m_Padding);
		auto size = m_Transform.size - glm::fvec2(m_Padding * 2.f);
		return Rect2D(pos, size);
	}

	auto Container::stretch() const -> EStretch {
		return m_Stretch;
	}

	auto Container::border() const -> const Border& {
		return m_Border;
	}

	auto Container::padding() const -> float {
		return m_Padding;
	}

} // namespace aby::eng::ui

namespace aby::eng::ui {

	Border::Border(float scalar, glm::fvec4 color) :
	    top(scalar),
	    right(scalar),
	    bottom(scalar),
	    left(scalar),
	    color(color) {
	}

	Border::Border(float top, float right, float bottom, float left, glm::fvec4 color) :
	    top(top),
	    right(right),
	    bottom(bottom),
	    left(left),
	    color(color) {
	}

	auto Border::set_scalar(float scalar) -> void {
		this->top    = scalar;
		this->right  = scalar;
		this->bottom = scalar;
		this->left   = scalar;
	}

} // namespace aby::eng::ui
