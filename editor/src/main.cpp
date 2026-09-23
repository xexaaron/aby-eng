#include "main.hpp"

#include "context.hpp"
#include "core/renderer.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <aby-eng/core/app.hpp>
#include <aby-eng/core/renderer.hpp>
#include <aby-win/window.hpp>

namespace aby::eng::editor {

	class ParentCloseFilter final : public QObject {
	public:
		ParentCloseFilter(win::Window& window, QObject* parent = nullptr) :
		    QObject(parent),
		    m_Window(window) {
		}
	protected:
		bool eventFilter(QObject*, QEvent* event) override {
			if (event->type() == QEvent::Close) {
				win::WindowClosedEvent close_event(m_Window.id());
				m_Window.close();
			}
			return false;
		}
	private:
		win::Window& m_Window;
	};

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({
	        .name    = "Abyss Engine",
	        .version = "1.0",
	        .argc    = argc,
	        .argv    = argv,
	        .win_cfg = win::Config{
	                               .name           = "Abyss Engine",
	                               .width          = 800,
	                               .height         = 600,
	                               .child          = true,
	                               .window_backend = win::EWindow::qt,
	                               .render_backend = win::ERenderBackend::vulkan,
	                               }
    }),
	    m_App(new QApplication(m_AppInfo.argc, m_AppInfo.argv)) {
	}

	auto EntryPoint::on_cmdl(argparse::ArgumentParser& parser) -> void {
	}

	auto EntryPoint::on_exec() -> void {
	}

	auto EntryPoint::on_create() -> void {
		auto* render_window = static_cast<QWindow*>(App::window()->native().backend_window);

		// The editor owns the actual Qt application window.
		auto* window = new QMainWindow();
		window->setWindowTitle(QStringLiteral("Abyss Engine"));
		window->resize(1280, 720);
		window->installEventFilter(new ParentCloseFilter(*App::window(), window));

		// Embed the renderer QWindow into the QMainWindow.
		auto* render_widget = QWidget::createWindowContainer(render_window, window);

		render_widget->setFocusPolicy(Qt::StrongFocus);
		window->setCentralWidget(render_widget);

		QIcon::setThemeName(QStringLiteral("oxygen"));

		auto* menubar = window->menuBar();

		auto* settings = menubar->addMenu(QStringLiteral("Settings"));

		settings->addAction(
		    QStringLiteral("Editor"),
		    [window, this] {
			auto* settings = new Settings(window);

			settings->add_page(
			    "Interface",
			    "Console",
			    m_Console->settings());

			settings->show();
		});

		m_Console = new Console(window);
		m_Logs    = new Logs(
		    rhi::Context::get().file_io()->path("editor.log"),
		    window);

		add_bottom_widget(window, m_Console);
		add_bottom_widget(window, m_Logs);

		window->statusBar()->showMessage("Ready");

		window->show();
	}

	auto EntryPoint::on_exit() -> void {
	}

	auto EntryPoint::add_bottom_widget(QMainWindow* window, QDockWidget* widget) -> void {
		static bool first_call = true;

		window->addDockWidget(
		    Qt::BottomDockWidgetArea,
		    widget);

		if (first_call) {
			first_call = false;
			return;
		}

		window->tabifyDockWidget(m_Console, widget);
	}

} // namespace aby::eng::editor

int main(int argc, char** argv) {
	using namespace aby::eng::editor;
	return EntryPoint::set<EntryPoint>(argc, argv);
}
