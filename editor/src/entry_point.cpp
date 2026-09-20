#include "entry_point.hpp"

#include "editor_app.hpp"

#include <QTimer>
#include <aby-eng/core/app.hpp>

namespace aby::eng::editor {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "Abyss Engine",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_cmdl(argparse::ArgumentParser& parser) -> void {
	}

	auto EntryPoint::on_exec() -> void {
		App::window()->set_visible(false);
		App::add_obj(Application::create(m_AppInfo.argc, m_AppInfo.argv));
	}

	auto EntryPoint::on_create() -> void {
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::editor
