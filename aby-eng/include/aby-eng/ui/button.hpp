#pragma once
#include "core/text.hpp"
#include "ui/element.hpp"
#include "ui/uitypes.hpp"

namespace aby::eng::ui {

	class ABY_API Button : public Element {
	public:
		Button(const Transform2D& transform, const Text& text, const ButtonStyle& style);

		static auto create(const Transform2D& transform, const Text& text, const ButtonStyle& style) -> ref<Button>;

		auto on_render() -> void override;
		auto on_event(win::Event& event) -> bool override;

		auto on_hovered(std::function<void()>&& fn) -> void;
		auto on_pressed(std::function<void()>&& fn) -> void;
		auto on_released(std::function<void()>&& fn) -> void;
	protected:
		auto on_mouse_moved(win::MouseMovedEvent& event) -> bool;
		auto on_mouse_pressed(win::MousePressedEvent& event) -> bool;
		auto on_mouse_released(win::MouseReleasedEvent& event) -> bool;
	private:
		Text m_Text;
		ButtonStyle m_Style;
		std::function<void()> m_OnHovered;
		std::function<void()> m_OnPressed;
		std::function<void()> m_OnReleased;
		EHoverState m_State;
		bool bHovered;
	};

} // namespace aby::eng::ui
