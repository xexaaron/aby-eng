#include "core/app.hpp"

#include "common-enums.hpp"
#include "core/entry.hpp"
#include "core/renderer.hpp"
#include "log.hpp"

#include <aby-win/backend/glfw/glfw-window.hpp>
#include <aby-win/common.hpp>
#include <aby-win/window.hpp>
#include <chrono>

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

	auto App::init(const AppInfo& info) -> bool {
		win::ILogger::set<WINLoggerInterface>();
		win::Config window_cfg;

		// TODO: Temporary arg handling
#ifdef __linux__
		rhi::EWindow window_backend = rhi::EWindow::wayland;
		for (int32_t i = 0; i < info.argc; i++) {
			if (std::strcmp(info.argv[i], "--render-doc") == 0) {
				window_backend = rhi::EWindow::x11;
			}
		}
#endif

		window_cfg.set_name(info.name)
		    .set_backends(win::EWindow::sdl, win::ERenderBackend::vulkan)
		    .set_size(800, 600)
		    .set_theme(win::ETheme::automatic)
		    .set_resizable(true)
		    .set_focused(true)
		    .set_visible(true)
		    .set_visible(true)
		    .set_render_doc(window_backend == rhi::EWindow::x11);

		m_Window = win::Window::create(window_cfg);

		m_Context = &rhi::Context::get();

		rhi::ContextParams ctx_cfg{
			.renderer_backend = rhi::ERenderer::vulkan,
#ifdef __linux__
			.window_backend = window_backend,
#else
			.window_backend = rhi::Ewindow::automatic,
#endif
			.native_window = m_Window->native().platform_window,
			.graphics      = {}
		};

#ifdef __linux__
		if (window_cfg.window_backend == win::EWindow::sdl) {
			ctx_cfg.wl_get_size_cb = [](uint32_t* w, uint32_t* h) {
				auto [x, y] = m_Window->size();
				*w          = x;
				*h          = y;
			};
		}
#endif

		m_Context->set_interface<RHILoggerInterface>();
		if (!m_Context->init(ctx_cfg)) {
			log_err("[eng] failed to initialize render context");
			return false;
		}

		expect(info.argv, "[eng] app info 'argv' was not set");
		m_Context->file_io()->set_cwd(fs::path(info.argv[0]).parent_path());
		m_Context->file_io()->set_cache_dir(m_Context->file_io()->cwd() / "cache");

		if (!Renderer2D::init()) {
			log_err("[eng] failed to initialize app renderer");
			return false;
		}

		m_Window->add_listener([](win::Event& event) -> bool {
			for (auto& object : m_Objects) {
				object->on_event(event);
			}
			return false;
		});

		return true;
	}

	auto App::deinit() -> void {
		Renderer2D::deinit();
		m_Context->deinit();
		auto _ = m_Window.release();
		Logger::shutdown();
	}

	auto App::run() -> void {
		win::Window& window = *m_Window.get();
		auto* renderer      = m_Context->renderer();

		EntryPoint::get()->on_exec();

		for (auto& object : m_Objects) {
			object->on_create();
		}

		m_State = EAppState::running;

		renderer->set_clear_color(rhi::Color(0.15f, 0.15f, 0.15f, 1.f));

		using clock     = std::chrono::steady_clock;
		auto last_frame = clock::now();

		while (!window.should_close()) {
			window.poll();

			if (window.should_close()) {
				break;
			}

			const auto now = clock::now();
			const Time deltatime(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_frame));
			last_frame = now;

			auto [w, h] = window.size();
			if (window.minimized() || w == 0 || h == 0) {
				continue;
			}

			for (auto& object : m_Objects) {
				object->on_tick(deltatime);
			}

			if (!Renderer2D::begin_frame()) {
				log_wrn("[eng] failed to begin frame");
				continue;
			}

			for (auto& object : m_Objects) {
				object->on_render();
			}

			Renderer2D::render();

			Renderer2D::end_frame();
		}

		m_State = EAppState::deinit;

		for (auto& object : m_Objects) {
			object->on_destroy();
		}

		EntryPoint::get()->on_exit();
	}

	auto App::window() -> win::Window* {
		return m_Window.get();
	}

	auto App::add_obj(ref<Object> object) -> void {
		expect(m_State != EAppState::deinit, "cannot add an object to the application during deinitalization");
		/// Objects have already been initialized but if we add one at runtime it needs to be created.
		if (m_State == EAppState::running) {
			object->on_create();
		}
		m_Objects.push_back(object);
	}

} // namespace aby::eng
