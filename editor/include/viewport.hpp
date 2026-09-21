#pragma once

#include <QWidget>
#include <qevent.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace aby::eng::editor {

	class Viewport : public QWidget {
		Q_OBJECT
	public:
		explicit Viewport(QWidget* parent = nullptr);
	protected:
		auto paintEvent(QPaintEvent* event) -> void override;
		auto resizeEvent(QResizeEvent* event) -> void override;
	private:
	};

} // namespace aby::eng::editor
