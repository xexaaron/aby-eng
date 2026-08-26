#include <aby-eng/core/app.hpp>

int main(int argc, char** argv) {
	aby::eng::AppInfo app_info{
		.name = "aby-eng-sandbox",
		.argc = argc,
		.argv = argv
	};

	aby::eng::App::init(app_info);

	auto& app = aby::eng::App::get();

	app.run();

	return 0;
}
