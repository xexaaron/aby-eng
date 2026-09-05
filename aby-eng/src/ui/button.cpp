#include "ui/button.hpp"

#include "core/renderer.hpp"
#include "misc/types.hpp"

namespace aby::eng::ui {

	Button::Button(const Transform2D& transform, const Text& text, const ButtonStyle& style) :
	    Element(transform),
	    m_Text(text),
	    m_Style(style),
	    m_OnHovered(nullptr),
	    m_OnPressed(nullptr),
	    m_OnReleased(nullptr),
	    bHovered(false),
	    m_State(EHoverState::normal) {
	}

	auto Button::create(const Transform2D& transform, const Text& text, const ButtonStyle& style) -> ref<Button> {
		return std::make_shared<Button>(transform, text, style);
	}

	auto Button::on_render() -> void {
		auto rect = resolved_layout();
		Border border;

		switch (m_State) {
			case EHoverState::normal: {
				Renderer2D::quad(Transform2D(rect.pos, rect.size), m_Style.normal.material);
				border = m_Style.normal.border;
				break;
			}
			case EHoverState::hovered: {
				Renderer2D::quad(Transform2D(rect.pos, rect.size), m_Style.hovered.material);
				border = m_Style.hovered.border;
				break;
			}
			case EHoverState::pressed: {
				Renderer2D::quad(Transform2D(rect.pos, rect.size), m_Style.pressed.material);
				border = m_Style.pressed.border;
				break;
			}
		}

		if (border.top > 0.f) {
			const float half = border.top / 2.f;

			Transform2D transform(
			    { rect.pos.x, rect.pos.y - half },
			    { rect.size.x, border.top },
			    1.f);

			Renderer2D::quad(transform, Material2D(border.color));
		}

		if (border.right > 0.f) {
			const float half = border.right / 2.f;

			Transform2D transform(
			    { rect.pos.x + rect.size.x - half, rect.pos.y },
			    { border.right, rect.size.y },
			    1.f);

			Renderer2D::quad(transform, Material2D(border.color));
		}

		if (border.bottom > 0.f) {
			const float half = border.bottom / 2.f;

			Transform2D transform(
			    { rect.pos.x, rect.pos.y + rect.size.y - half },
			    { rect.size.x, border.bottom },
			    1.f);

			Renderer2D::quad(transform, Material2D(border.color));
		}

		if (border.left > 0.f) {
			const float half = border.left / 2.f;

			Transform2D transform(
			    { rect.pos.x - half, rect.pos.y },
			    { border.left, rect.size.y },
			    1.f);

			Renderer2D::quad(transform, Material2D(border.color));
		}

		if (!m_Text.empty()) {
			auto center  = rect.center();
			auto size    = m_Text.size();
			size        /= 2;
			center      -= size;

			Text2D txt(m_Text.view());
			Renderer2D::text(center, m_Text.font(), txt);
		}

		Element::on_render();
	}

	auto Button::on_event(win::Event& event) -> bool {
		win::EventDispatcher dsp(event);
		dsp.dispatch(&Button::on_mouse_moved, this);
		dsp.dispatch(&Button::on_mouse_pressed, this);
		dsp.dispatch(&Button::on_mouse_released, this);
		// TODO: edit aby-win dispatch to return if the event consumed or not
		//       or return win::EventDispatcher::consumed() alternatively.
		return Element::on_event(event);
	}

	auto Button::on_hovered(std::function<void()>&& fn) -> void {
		m_OnHovered = std::move(fn);
	}

	auto Button::on_pressed(std::function<void()>&& fn) -> void {
		m_OnPressed = std::move(fn);
	}

	auto Button::on_released(std::function<void()>&& fn) -> void {
		m_OnReleased = std::move(fn);
	}

	auto Button::on_mouse_moved(win::MouseMovedEvent& event) -> bool {
		auto x      = event->x;
		auto y      = event->y;
		Rect2D rect = this->resolved_layout();

		if (rect.contains(x, y)) {
			bHovered = true;
			m_State  = EHoverState::hovered;
			if (m_OnHovered) {
				m_OnHovered();
			}
			return true;
		}

		m_State  = EHoverState::normal;
		bHovered = false;
		return false;
	}

	auto Button::on_mouse_pressed(win::MousePressedEvent& event) -> bool {
		if (event->button == win::EMouseButton::left) {
			if (bHovered) {
				m_State = EHoverState::pressed;
				if (m_OnPressed) {
					m_OnPressed();
				}
				return true;
			}
		}
		return false;
	}

	auto Button::on_mouse_released(win::MouseReleasedEvent& event) -> bool {
		if (event->button == win::EMouseButton::left) {
			if (bHovered) {
				if (m_OnReleased) {
					m_OnReleased();
				}
				m_State = EHoverState::hovered;
				return true;
			}
		}
		return false;
	}

} // namespace aby::eng::ui
