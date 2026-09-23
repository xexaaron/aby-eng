#include "entry_point.hpp"

#include "moving_quad.hpp"

namespace aby::eng::sandbox {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "aby-eng-sandbox",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_exec() -> void {
		App::add_obj(std::make_shared<MovingQuad>());
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::sandbox
