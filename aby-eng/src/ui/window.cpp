#include "ui/window.hpp"

#include "event.hpp"
#include "log.hpp"
#include "misc/registry.hpp"
#include "misc/types.hpp"
#include "ui/element.hpp"
#include "window.hpp"

namespace aby::eng::ui {

	Window::Window(const Text& title, const Rect2D& area) :
	    Element(Transform2D(area.pos, area.size)),
	    m_Title(title),
	    m_Window(nullptr) {
	}

	auto Window::create(const Text& title, const Rect2D& area) -> ref<Window> {
		return std::make_shared<Window>(title, area);
	}

	auto Window::on_create() -> void {
		static bool bChild = false;

		win::Config cfg;
		cfg.set_backends(Registry::get<"win-backend">(), Registry::get<"win-render-backend">())
		    .set_name(m_Title.view())
		    .set_size(m_Transform.size.x, m_Transform.size.y)
		    .set_theme(win::ETheme::automatic)
		    .set_resizable(true)
		    .set_focused(true)
		    .set_visible(true)
		    .set_visible(true)
		    .set_render_doc(Registry::get("app-flag-render-doc"))
		    .set_child(true);

		if (!bChild) {
			bChild = true;
		}

		m_Window = win::Window::create(cfg);

		m_Window->add_listener([this](win::Event& event) -> bool {
			return on_window_event(event);
		});

		Element::on_create();
	}

	auto Window::on_tick(const Time& dt) -> void {
		if (!m_Window->should_close()) {
			Element::on_tick(dt);
		}
	}

	auto Window::on_event(win::Event& event) -> bool {
		if (event.window() == 0) {
			log_inf("event: {}", event);
		}
		/// We dont recieve events normally as a child window.
		return false;
	}

	auto Window::on_destroy() -> void {
		auto _ = m_Window.release();
	}

	auto Window::on_window_event(win::Event& event) -> bool {
		win::EventDispatcher dsp(event);
		dsp.dispatch(&Window::on_window_close, this);
		return false;
	}

	auto Window::on_window_close(win::WindowClosedEvent& event) -> bool {
		m_Window->close();
		return false;
	}

} // namespace aby::eng::ui
