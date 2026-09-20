#include "editor_window.hpp"

#include "editor_settings.hpp"

#include <QDockWidget>
#include <QIcon>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <aby-eng/core/app.hpp>
#include <aby-rhi/context.hpp>
#include <mutex>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qlogging.h>
#include <qmainwindow.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpalette.h>
#include <qwindowdefs.h>

#ifdef emit
#	define __qt_emit_stored__ emit
#	undef emit
#	include <aby-eng/log.hpp>
#else
#	include <aby-eng/log.hpp>
#endif

#include <QAnsiTextEdit.h>
#include <qapplication.h>
#include <qdockwidget.h>
#include <qtermwidget.h>

namespace aby::eng::editor::detail {

	void qt_message_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

} // namespace aby::eng::editor::detail

auto operator""_q(const char* s, std::size_t n) -> QString {
	return QString::fromUtf8(s, qsizetype(n));
}

namespace aby::eng::editor {

	Window::Window(QWidget* parent) :
	    QMainWindow(parent) {
		qInstallMessageHandler(detail::qt_message_handler);
		setWindowTitle("Abyss Engine");
		resize(800, 600);

		QIcon::setThemeName("oxygen"_q);

		create_menus();
		create_viewport();
		create_docks();

		statusBar()->showMessage("Ready");
	}

	void Window::create_menus() {
		auto* menubar = menuBar();

		auto* settings = menubar->addMenu("Settings"_q);

		settings->addAction("Editor"_q, [this] {
			auto settings = new Settings(this);
			settings->show();
		});
	}

	void Window::create_docks() {
		auto* viewport = new QLabel("<Viewport>");

		viewport->setAlignment(Qt::AlignCenter);
		viewport->setMinimumSize(400, 300);

		setCentralWidget(viewport);
	}

	void Window::create_viewport() {
		auto* console = create_console();
		addDockWidget(Qt::BottomDockWidgetArea, console);

		auto* logs = create_logs();
		addDockWidget(Qt::BottomDockWidgetArea, logs);

		tabifyDockWidget(console, logs);
	}

	auto Window::create_logs() -> QDockWidget* {
		auto [dock, logs] = docked<QAnsiTextEdit>("Log");
		logs->setReadOnly(true);
		logs->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse);
		QPalette palette = logs->palette();

		palette.setColor(QPalette::Base, QColor(44, 44, 44));

		logs->setPalette(palette);

		auto sink = LogSink([logs](const LogRecord& record) {
			const auto msg = QString::fromStdString(record.msg);

			QMetaObject::invokeMethod(
			    logs,
			    [logs, msg] {
				logs->appendAnsiText(msg);
			},
			    Qt::QueuedConnection);
		}, ELogLevel::err);

		auto internal = Logger::get(ELogger::internal);
		auto client   = Logger::get(ELogger::client);

		internal->add_sink(sink);
		client->add_sink(sink);

		return dock;
	}

	auto Window::create_console() -> QDockWidget* {
		auto cwd   = rhi::Context::get().file_io()->cwd();
		auto csdir = cwd / "ui" / "terminal_colorschemes";

		auto [dock, console] = docked<QTermWidget>("Console");

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

		return dock;
	}

	auto Window::event(QEvent* event) -> bool {
		if (event->type() == QEvent::Close) {
			App::exit();
		}

		return QMainWindow::event(event);
	}

} // namespace aby::eng::editor

namespace aby::eng::editor::detail {

	void qt_message_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
		switch (type) {
			case QtDebugMsg:
				log_dev("[qt] {}", msg.toStdString());
				break;
			case QtInfoMsg:
				log_inf("[qt] {}", msg.toStdString());
				break;
			case QtWarningMsg:
				log_wrn("[qt] {}", msg.toStdString());
				break;
			case QtCriticalMsg:
				log_err("[qt] {}", msg.toStdString());
				break;
			case QtFatalMsg:
				Logger::get(ELogger::internal)->log(ELogLevel::ast, "[qt] {}:({}) @ {}", context.file, context.line, context.function);
				Logger::get(ELogger::internal)->log(ELogLevel::ast, "[qt] {}", msg.toStdString());
				break;
		}
	}

} // namespace aby::eng::editor::detail

#ifdef __qt_emit_stored__
#	define emit __qt_emit_stored__
#	undef __qt_emit_stored__
#endif

