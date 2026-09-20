#include "editor_app.hpp"

#include <qlogging.h>

#ifdef emit
#	define __qt_emit_stored__ emit
#	undef emit
#	include <aby-eng/log.hpp>
#else
#	include <aby-eng/log.hpp>
#endif

namespace aby::eng::editor {

	Application::Application(int argc, char** argv) :
	    m_Argc(argc),
	    m_Argv(argv),
	    m_App(m_Argc, m_Argv) {
	}

	auto Application::create(int argc, char** argv) -> ref<Application> {
		return std::make_shared<Application>(argc, argv);
	}

	auto Application::on_create() -> void {
		m_Window.show();
	}

	auto Application::on_tick(const Time& deltatime) -> void {
		m_App.processEvents();
	}

	auto Application::on_render() -> void {
	}

	auto Application::on_event(win::Event& event) -> bool {
		return false;
	}

	auto Application::on_destroy() -> void {
	}

} // namespace aby::eng::editor

#ifdef __qt_emit_stored__
#	define emit __qt_emit_stored__
#	undef __qt_emit_stored__
#endif
