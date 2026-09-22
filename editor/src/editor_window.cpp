#include "editor_window.hpp"

#include "editor_common.hpp"
#include "widgets/editor_settings.hpp"

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

namespace aby::eng::editor {

	Window::Window(QWidget* parent) :
	    QMainWindow(parent),
	    m_Console(new Console(this)),
	    m_Logs(new Logs(rhi::Context::get().file_io()->path("editor.log"), this)) {
		setWindowTitle("Abyss Engine");
		resize(800, 600);

		QIcon::setThemeName("oxygen"_q);

		create_menus();

		add_bottom_widget(m_Console);
		add_bottom_widget(m_Logs);

		create_docks();

		statusBar()->showMessage("Ready");
	}

	void Window::create_menus() {
		auto* menubar = menuBar();

		auto* settings = menubar->addMenu("Settings"_q);

		settings->addAction("Editor"_q, [this] {
			auto settings = new Settings(this);
			settings->add_page("Interface", "Console", m_Console->settings());
			settings->show();
		});
	}

	void Window::create_docks() {
		auto* viewport = new QLabel("<Viewport>");

		viewport->setAlignment(Qt::AlignCenter);
		viewport->setMinimumSize(400, 300);

		setCentralWidget(viewport);
	}

	auto Window::add_bottom_widget(QDockWidget* widget) -> void {
		static bool first_call = true;

		addDockWidget(Qt::BottomDockWidgetArea, widget);

		if (first_call) {
			first_call = false;
			return;
		}

		tabifyDockWidget(m_Console, widget);
	}

	auto Window::event(QEvent* event) -> bool {
		if (event->type() == QEvent::Close) {
			App::exit();
		}

		return QMainWindow::event(event);
	}

} // namespace aby::eng::editor

#ifdef __qt_emit_stored__
#	define emit __qt_emit_stored__
#	undef __qt_emit_stored__
#endif

