#pragma once

#include "widgets/editor_logs.hpp"

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
	private:
		auto add_bottom_widget(QDockWidget* widget) -> void;
	private:
		Console* m_Console;
		Logs* m_Logs;
	};

} // namespace aby::eng::editor
