#pragma once

#include "editor_settings.hpp"

#include <QWidget>
#include <qdockwidget.h>
#include <qtermwidget.h>
#include <qwidget.h>

namespace aby::eng::editor {

	class Console : public QDockWidget {
		Q_OBJECT
	public:
		explicit Console(QWidget* parent = nullptr);

		auto settings() -> ConsoleSettings*;
		auto term() -> QTermWidget*;
	private:
		ConsoleSettings* m_Settings;
	};

} // namespace aby::eng::editor
