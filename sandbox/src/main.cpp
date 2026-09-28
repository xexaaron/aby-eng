#include "main.hpp"

#include "moving_quad.hpp"

namespace aby::eng::sandbox {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "aby-eng-sandbox",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_exec() -> void {
		create<MovingQuad>();
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::sandbox

int main(int argc, char** argv) {
	using namespace aby::eng::sandbox;
	return EntryPoint::exec<EntryPoint>(argc, argv);
}
