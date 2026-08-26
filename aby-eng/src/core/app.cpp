#include "core/app.hpp"

#include "log.hpp"

#include <aby-win/common.hpp>
namespace aby::eng {

	class RHILoggerInterface : public rhi::ILogger {
	public:
		auto log(rhi::ELogLevel level, const std::string& msg) -> void override {
			switch (level) {
				case rhi::ELogLevel::debug:
					log_dev("[rhi] {}", msg);
					break;
				case rhi::ELogLevel::trace:
					log_trc("[rhi] {}", msg);
					break;
				case rhi::ELogLevel::info:
					log_inf("[rhi] {}", msg);
					break;
				case rhi::ELogLevel::warn:
					log_wrn("[rhi] {}", msg);
					break;
				case rhi::ELogLevel::error:
					log_err("[rhi] {}", msg);
					break;
				case rhi::ELogLevel::fatal:
					// The library logs multiple times to fatal level per assert
					// We want the logger to immediately dump these asserts instead of queueing them
					// into the mulithreaded log handler
					Logger::get(ELogger::internal)->log(ELogLevel::ast, "[rhi] {}", msg);
					break;
			}
		}
	private:
	};

	class WINLoggerInterface : public win::ILogger {
	public:
		auto log(win::ELogLevel level, const std::string& msg) -> void {
			switch (level) {
				case win::ELogLevel::debug:
					log_dev("[win] {}", msg);
					break;
				case win::ELogLevel::trace:
					log_trc("[win] {}", msg);
					break;
				case win::ELogLevel::info:
					log_inf("[win] {}", msg);
					break;
				case win::ELogLevel::warn:
					log_wrn("[win] {}", msg);
					break;
				case win::ELogLevel::error:
					log_err("[win] {}", msg);
					break;
				case win::ELogLevel::fatal:
					// The library logs multiple times to fatal level per assert.
					// We want the logger to immediately dump these asserts instead of queueing them
					// into the mulithreaded log handler
					Logger::get(ELogger::internal)->log(ELogLevel::ast, "[win] {}", msg);
					break;
			}
		}
	private:
	};

} // namespace aby::eng

namespace aby::eng {

	static std::unique_ptr<App> s_App;

	auto App::init(const AppInfo& info) -> bool {
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

		win::ILogger::set<WINLoggerInterface>();

		s_App.reset(new App(info));
		return s_App != nullptr;
	}

	auto App::get() -> App& {
		return *s_App.get();
	}

	App::App(const AppInfo& info) :
	    m_Window(nullptr),
	    m_Context(nullptr) {
		win::Config window_cfg;

		window_cfg.set_name(info.name)
		    .set_backends(win::EWindow::sdl, win::ERenderBackend::vulkan)
		    .set_size(800, 600)
		    .set_theme(win::ETheme::automatic)
		    .set_resizable(true)
		    .set_focused(true)
		    .set_visible(true)
		    .set_visible(true);

		m_Window = win::Window::create(window_cfg);

		m_Context = &rhi::Context::get();

		rhi::ContextParams ctx_cfg{
			.renderer_backend = rhi::ERenderer::vulkan,
			.window_backend   = rhi::EWindow::win32, // TODO: not hardcode this
			.native_window    = m_Window->native().platform_window,
			.graphics         = {}
		};

		m_Context->set_interface<RHILoggerInterface>();
		if (!m_Context->init(ctx_cfg)) {
			log_err("[eng] failed to initialize render context");
			return;
		}

		expect(info.argv, "[eng] app info 'argv' was not set");
		m_Context->file_io()->set_cwd(fs::path(info.argv[0]).parent_path());
		m_Context->file_io()->set_cache_dir(m_Context->file_io()->cwd() / "cache");
	}

	App::~App() {
		m_Context->deinit();
		m_Window.release();
		Logger::shutdown();
	}

	auto App::run() -> void {
		win::Window& window = *m_Window.get();
		auto* renderer      = m_Context->renderer();

		renderer->set_clear_color(rhi::Color(0.15f, 0.15f, 0.15f, 1.f));

		while (!window.should_close()) {
			window.poll();

			if (window.should_close()) {
				break;
			}

			if (!renderer->on_begin()) {
				continue;
			}

			renderer->on_end();
		}
	}

} // namespace aby::eng
