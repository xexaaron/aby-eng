#include "widgets/editor_console.hpp"

#include "editor_common.hpp"

#include <QApplication>
#include <aby-rhi/aby-rhi.hpp>
#include <qtermwidget.h>
#include <qwidget.h>

namespace aby::eng::editor {

	Console::Console(QWidget* parent) :
	    QDockWidget("Console", parent),
	    m_Settings(new ConsoleSettings) {
		auto* console = new QTermWidget();

		auto cwd   = rhi::Context::get().file_io()->cwd();
		auto csdir = cwd / "ui" / "terminal_colorschemes";

		console->setScrollBarPosition(QTermWidget::ScrollBarRight);
		console->setWorkingDirectory(QString::fromStdString(cwd.string()));
		console->addCustomColorSchemeDir(QString::fromStdString(csdir.string()));
		console->setColorScheme("DarkPastels");

		QFont font = QApplication::font();
#ifdef Q_OS_MACOS
		font.setFamily("Monaco"_q);
#elif defined(Q_WS_QWS)
		font.setFamily("fixed"_q);
#else
		font.setFamily("Monospace"_q);
#endif
		font.setPointSize(10);

		console->setTerminalFont(font);
		console->setShellProgram(
		    "/bin/bash -c "
		    "\""
		    "alias ls='eza -al --color=always --group-directories-first'; "
		    "exec bash"
		    "\"");

		setWidget(console);
	}

	auto Console::settings() -> ConsoleSettings* {
		return m_Settings;
	}

	auto Console::term() -> QTermWidget* {
		return static_cast<QTermWidget*>(this->widget());
	}

} // namespace aby::eng::editor
