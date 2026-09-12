#include "entry_point.hpp"

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
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::editor
