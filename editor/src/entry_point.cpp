#include "entry_point.hpp"

#include "ui/rml.hpp"

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
		auto [w, h] = App::window()->size();
		m_Renderer  = rml::Renderer::create({ w, h });
		App::add_obj(m_Renderer);
	}

	auto EntryPoint::on_create() -> void {
		m_Renderer->load("index.rml");
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::editor
