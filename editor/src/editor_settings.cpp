#include "editor_settings.hpp"

#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <qdialog.h>

namespace aby::eng::editor {

	Settings::Settings(QWidget* parent) :
	    QDialog(parent) {
		setWindowTitle(tr("Settings"));
		resize(800, 500);

		m_Tree = new QTreeWidget(this);
		m_Tree->setHeaderHidden(true);

		m_Pages = new QStackedWidget(this);

		auto* splitter = new QSplitter(Qt::Horizontal, this);
		splitter->addWidget(m_Tree);
		splitter->addWidget(m_Pages);

		splitter->setSizes({ 800 * 30 / 100,
		                     800 * 70 / 100 });

		auto* layout = new QVBoxLayout(this);
		layout->addWidget(splitter);

		connect(
		    m_Tree,
		    &QTreeWidget::currentItemChanged,
		    this,
		    [this](QTreeWidgetItem* current) {
			if (!current)
				return;

			auto* page = current->data(0, Qt::UserRole)
			                 .value<QWidget*>();

			if (page)
				m_Pages->setCurrentWidget(page);
		});
	}

	auto Settings::add_page(const QString& category, QWidget* page) -> void {
		auto* item = new QTreeWidgetItem(m_Tree);
		item->setText(0, category);
		item->setData(
		    0,
		    Qt::UserRole,
		    QVariant::fromValue(page));

		m_Pages->addWidget(page);

		if (!m_Tree->currentItem())
			m_Tree->setCurrentItem(item);
	}

} // namespace aby::eng::editor
