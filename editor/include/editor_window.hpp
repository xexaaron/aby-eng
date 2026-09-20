#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <aby-eng/core/object.hpp>
#include <qdockwidget.h>
#include <qmainwindow.h>
#include <qobject.h>

namespace aby::eng::editor {

	class Window final : public QMainWindow {
		Q_OBJECT
	public:
		explicit Window(QWidget* parent = nullptr);

		auto event(QEvent* event) -> bool override;
	private:
		auto create_menus() -> void;
		auto create_docks() -> void;
		auto create_viewport() -> void;
		auto create_console() -> QDockWidget*;
		auto create_logs() -> QDockWidget*;
	private:
		template <typename T, typename... Args>
		auto docked(const std::string& name, Args&&... args) -> std::pair<QDockWidget*, T*> {
			auto* widget      = new T(std::forward<Args>(args)...);
			auto* widget_dock = new QDockWidget(QString::fromStdString(name), this);
			widget_dock->setWidget(widget);
			return { widget_dock, widget };
		}
	private:
	};

} // namespace aby::eng::editor
