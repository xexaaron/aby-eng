#pragma once

#include <QDialog>
#include <QObject>
#include <QStackedWidget>
#include <QTreeWidget>
#include <qwidget.h>

namespace aby::eng::editor {

	class Settings : public QDialog {
		Q_OBJECT
	public:
		explicit Settings(QWidget* parent = nullptr);

		auto add_page(const QString& category, QWidget* page) -> void;
		auto add_page(const QString& category, const QString& subcategory, QWidget* page) -> void;
	private:
		QTreeWidget* m_Tree     = nullptr;
		QStackedWidget* m_Pages = nullptr;
	};

} // namespace aby::eng::editor
