#include "ui/containers/container.hpp"

#include "core/renderer.hpp"

namespace aby::eng::ui {

	Container::Container(Transform2D transform, ELayout layout, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) :
	    Element(transform),
	    m_Layout(layout),
	    m_Direction(direction),
	    m_Spacing(spacing),
	    m_Padding(padding),
	    m_Border(border),
	    m_Stretch(stretch) {
	}

	auto Container::create(Transform2D transform, ELayout layout, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) -> ref<Container> {
		return std::make_shared<Container>(transform, layout, direction, spacing, padding, border, stretch);
	}

	auto Container::on_tick(const Time& deltatime) -> void {
		const Rect2D rect    = available_rect();
		const auto& children = this->children();

		if (children.empty()) {
			Element::on_tick(deltatime);
			return;
		}

		const bool horizontal         = m_Layout == ELayout::horizontal;
		const float primary_size      = horizontal ? rect.size.x : rect.size.y;
		const float cross_size        = horizontal ? rect.size.y : rect.size.x;
		const float total_spacing     = m_Spacing * static_cast<float>(children.size() - 1);
		const float available_primary = std::max(0.f, primary_size - total_spacing);
		const float child_primary     = available_primary / static_cast<float>(children.size());
		float cursor                  = m_Direction == EDirection::forward ? 0.f : primary_size;

		for (auto& child : children) {
			if (!child)
				continue;

			glm::fvec2 slot_size;

			if (horizontal) {
				slot_size = {
					child_primary,
					cross_size
				};
			} else {
				slot_size = {
					cross_size,
					child_primary
				};
			}

			glm::fvec2 slot_pos;

			if (m_Direction == EDirection::forward) {
				if (horizontal) {
					slot_pos = {
						rect.pos.x + cursor,
						rect.pos.y
					};
				} else {
					slot_pos = {
						rect.pos.x,
						rect.pos.y + cursor
					};
				}

				cursor += child_primary + m_Spacing;
			} else {
				cursor -= child_primary;

				if (horizontal) {
					slot_pos = {
						rect.pos.x + cursor,
						rect.pos.y
					};
				} else {
					slot_pos = {
						rect.pos.x,
						rect.pos.y + cursor
					};
				}

				cursor -= m_Spacing;
			}

			const Rect2D slot(slot_pos, slot_size);

			child->set_resolved_layout(resolve_child_layout(*child, slot));
		}

		Element::on_tick(deltatime);
	}

	auto Container::on_render() -> void {
		const auto rect  = resolved_layout();
		const auto& pos  = rect.pos;
		const auto& size = rect.size;

		Material2D mat(m_Border.color);

		Element::on_render();

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

	auto Container::set_spacing(float spacing) -> void {
		m_Spacing = spacing;
	}

	auto Container::available_rect() const -> Rect2D {
		const auto& rect = resolved_layout();
		const auto pos   = rect.pos + glm::fvec2(m_Padding);
		const auto size  = rect.size - glm::fvec2(m_Padding * 2.f);
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

	auto Container::spacing() const -> float {
		return m_Spacing;
	}

	auto Container::resolve_child_layout(const Element& child, const Rect2D& rect) const -> Rect2D {
		switch (m_Stretch) {
			case EStretch::fill: {
				return rect;
			}
			case EStretch::fit: {
				const auto requested = child.transform().size;

				const float sx = rect.size.x / requested.x;
				const float sy = rect.size.y / requested.y;

				const float scale = std::min(sx, sy);

				const glm::fvec2 size = requested * scale;

				const glm::fvec2 pos = {
					rect.pos.x + (rect.size.x - size.x) * 0.5f,
					rect.pos.y + (rect.size.y - size.y) * 0.5f
				};

				return Rect2D(pos, size);
			}
			case EStretch::cover: {
				const auto requested = child.transform().size;

				const float sx = rect.size.x / requested.x;
				const float sy = rect.size.y / requested.y;

				const float scale = std::max(sx, sy);

				const glm::fvec2 size = requested * scale;

				const glm::fvec2 pos = {
					rect.pos.x + (rect.size.x - size.x) * 0.5f,
					rect.pos.y + (rect.size.y - size.y) * 0.5f
				};

				return Rect2D(pos, size);
			}
		}

		return Rect2D();
	}

} // namespace aby::eng::ui

namespace aby::eng::ui {

	HContainer::HContainer(Transform2D transform, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) :
	    Container(transform, ELayout::horizontal, direction, spacing, padding, border, stretch) {
	}

	auto HContainer::create(Transform2D transform, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) -> ref<HContainer> {
		return std::make_shared<HContainer>(transform, direction, spacing, padding, border, stretch);
	}

	VContainer::VContainer(Transform2D transform, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) :
	    Container(transform, ELayout::vertical, direction, spacing, padding, border, stretch) {
	}

	auto VContainer::create(Transform2D transform, EDirection direction, float spacing, float padding, const Border& border, EStretch stretch) -> ref<VContainer> {
		return std::make_shared<VContainer>(transform, direction, spacing, padding, border, stretch);
	}

} // namespace aby::eng::ui
