#include "entry_point.hpp"

#include "core/renderer.hpp"
#include "editor_app.hpp"
#include "window.hpp"

#include <QGuiApplication>
#include <aby-eng/core/app.hpp>
#include <aby-eng/core/renderer.hpp>
#include <qnativeinterface.h>

namespace aby::eng::editor {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "Abyss Engine",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv,
		                  .win_cfg = win::Config{
	                          .window_backend = win::EWindow::qt } }),
	    m_App(Application::create(m_AppInfo.argc, m_AppInfo.argv)) {
	}

	auto EntryPoint::on_cmdl(argparse::ArgumentParser& parser) -> void {
	}

	auto EntryPoint::on_exec() -> void {
		// App::window()->set_visible(false);
		App::add_obj(m_App);
	}

	auto EntryPoint::on_create() -> void {
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::editor
