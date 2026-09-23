#include "argparse/argparse.hpp"
#include "widgets/editor_console.hpp"
#include "widgets/editor_logs.hpp"
#include "widgets/editor_settings.hpp"
#include "window.hpp"

#include <QApplication>
#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-win/aby-win.hpp>
#include <qapplication.h>

namespace aby::eng::editor {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv);

		auto on_cmdl(argparse::ArgumentParser& parser) -> void override;
		auto on_exec() -> void override;
		auto on_create() -> void override;
		auto on_exit() -> void override;
	private:
		auto add_bottom_widget(QMainWindow* window, QDockWidget* widget) -> void;
	private:
		QApplication* m_App;
		Console* m_Console;
		Logs* m_Logs;
	};

} // namespace aby::eng::editor
