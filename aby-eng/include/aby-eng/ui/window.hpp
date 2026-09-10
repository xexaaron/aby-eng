#pragma once
#include "core/text.hpp"
#include "element.hpp"
#include "event.hpp"
#include "misc/types.hpp"

#include <aby-win/window.hpp>

namespace aby::eng::ui {

	class Window : public Element {
	public:
		Window(const Text& title, const Rect2D& area);
		static auto create(const Text& title, const Rect2D& area) -> ref<Window>;

		auto on_create() -> void override;
		auto on_event(win::Event& event) -> bool override;
		auto on_tick(const Time& dt) -> void override;
		auto on_destroy() -> void override;
	protected:
		auto on_window_event(win::Event& event) -> bool;
		auto on_window_close(win::WindowClosedEvent& event) -> bool;
	private:
		Text m_Title;
		unique<win::Window> m_Window;
	};

} // namespace aby::eng::ui
