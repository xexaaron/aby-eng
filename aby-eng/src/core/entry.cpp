#include "core/entry.hpp"

#include "log.hpp"

#if !defined(_NDEBUG) && defined(_MSC_VER)
#	include <crtdbg.h>
#endif

namespace aby::eng {

	auto EntryPoint::get() -> ref<EntryPoint> {
		return m_EntryPoint;
	}

	auto EntryPoint::init(int argc, char** argv) -> i32 {
#if !defined(_NDEBUG) && defined(_MSC_VER)
		_CrtSetDbgFlag(
		    _CRTDBG_ALLOC_MEM_DF |
		    _CRTDBG_LEAK_CHECK_DF);
#endif
		Logger::get(ELogger::client)
		    ->set_level_info(ELogLevel::log, { .color = "\033[1;32m", .prefix = "| LOG |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::warn, { .color = "\033[33m", .prefix = "| WRN |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::err, { .color = "\033[31m", .prefix = "| ERR |", .stream = &std::cerr })
		    ->set_timestamp(true);
		Logger::get(ELogger::internal)
		    ->set_level_info(ELogLevel::trace, { .color = "\033[1;30m", .prefix = "| TRC |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::todo, { .color = "\033[38;2;255;182;193m", .prefix = "| TDO |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::log, { .color = "\033[1;32m", .prefix = "| LOG |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::warn, { .color = "\033[33m", .prefix = "| WRN |", .stream = &std::cout })
		    ->set_level_info(ELogLevel::err, { .color = "\033[31m", .prefix = "| ERR |", .stream = &std::cerr })
		    ->set_level_info(ELogLevel::ast, { .color = "\033[31m", .prefix = "| AST |", .stream = &std::cerr })
		    ->set_level_info(ELogLevel::dev, { .color = "\033[1;36m", .prefix = "| DEV |", .stream = &std::cout })
		    ->set_timestamp(true);

		Logger::run();

		auto entry_point = EntryPoint::get();
		if (!entry_point) {
			log_err("[eng] entry point was not set.");
			return 1;
		}

		auto app_info = entry_point->init();

		if (!App::init(app_info)) {
			log_err("[eng] failed to initialize the app");
		}

		auto& app = App::get();
		app.run();

		return 0;
	}

} // namespace aby::eng
