#pragma once

#include "editor_window.hpp"

#include <QApplication>
#include <aby-eng/core/object.hpp>
#include <qapplication.h>

namespace aby::eng::editor {

	class Application : public Object {
	public:
		Application(int argc, char** argv);

		static auto create(int argc, char** argv) -> ref<Application>;

		auto window() -> Window&;

		auto on_create() -> void override;
		auto on_tick(const Time& deltatime) -> void override;
		auto on_render() -> void override;
		auto on_event(win::Event& event) -> bool override;
		auto on_destroy() -> void override;
	private:
		int m_Argc;
		char** m_Argv;
		QApplication m_App;
		Window m_Window;
	};

} // namespace aby::eng::editor
