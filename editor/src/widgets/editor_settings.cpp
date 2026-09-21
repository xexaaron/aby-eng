#include "widgets/editor_settings.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSplitter>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qdialog.h>
#include <qvariant.h>
#include <qwidget.h>

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

	auto Settings::add_page(const QString& category, const QString& subcategory, QWidget* page) -> void {
		QTreeWidgetItem* category_item = nullptr;

		// Find existing category.
		for (int i = 0; i < m_Tree->topLevelItemCount(); ++i) {
			auto* item = m_Tree->topLevelItem(i);

			if (item->text(0) == category) {
				category_item = item;
				break;
			}
		}

		// Create category if it doesn't exist.
		if (!category_item)
			category_item = new QTreeWidgetItem(m_Tree, { category });

		// Add the page as a child of the category.
		auto* item = new QTreeWidgetItem(category_item);
		item->setText(0, subcategory);
		item->setData(
		    0,
		    Qt::UserRole,
		    QVariant::fromValue(page));

		m_Pages->addWidget(page);

		category_item->setExpanded(true);

		if (!m_Tree->currentItem())
			m_Tree->setCurrentItem(item);
	}

} // namespace aby::eng::editor

namespace aby::eng::editor {

	SettingsMenu::SettingsMenu(const QString& name, QWidget* parent) :
	    QWidget(parent),
	    m_Settings("Abyss Engine", "Abyss Engine Editor"),
	    m_Name(name) {
		m_Layout = new QGridLayout(this);

		m_Layout->setContentsMargins(8, 8, 8, 8);
		m_Layout->setHorizontalSpacing(8);
		m_Layout->setVerticalSpacing(2);

		// Labels take the space they need.
		m_Layout->setColumnStretch(0, 1);

		// Values get the remaining space.
		m_Layout->setColumnStretch(1, 2);
	}

	auto SettingsMenu::settings() -> QSettings& {
		return m_Settings;
	}

	auto SettingsMenu::add(const QString& name, QVariant default_value, const QString& help) -> void {
		const auto k = key(name);

		if (!m_Settings.contains(k)) {
			if (default_value.typeId() == QMetaType::QStringList) {
				const auto values = default_value.toStringList();

				if (values.isEmpty())
					return;

				m_Settings.setValue(k, values.front());
			} else {
				m_Settings.setValue(k, default_value);
			}
		}

		const auto value = m_Settings.value(k);

		QWidget* widget = nullptr;

		if (default_value.typeId() == QMetaType::QStringList) {
			const auto values = default_value.toStringList();

			if (!values.isEmpty())
				widget = create_widget(k, values, value.toString());
		} else {
			widget = create_widget(k, value);
		}

		if (!widget)
			return;

		widget->setMinimumWidth(180);

		const int row = m_Layout->rowCount();

		auto* label = new QLabel(name, this);
		label->setToolTip(help);

		m_Layout->addWidget(label, row, 0);
		m_Layout->addWidget(widget, row, 1);

		// Align the label vertically with the control.
		m_Layout->setAlignment(label, Qt::AlignVCenter);
	}

	auto SettingsMenu::create_widget(const QString& key, const QVariant& value) -> QWidget* {
		switch (value.typeId()) {
			case QMetaType::Bool: {
				auto* widget = new QCheckBox(this);
				widget->setChecked(value.toBool());

				connect(widget, &QCheckBox::toggled, this, [this, key](bool value) {
					m_Settings.setValue(key, value);
					emit setting_changed(key, value);
				});

				return widget;
			}

			case QMetaType::Int: {
				auto* widget = new QSpinBox(this);
				widget->setValue(value.toInt());

				connect(widget, qOverload<int>(&QSpinBox::valueChanged), this, [this, key](int value) {
					m_Settings.setValue(key, value);
					emit setting_changed(key, value);
				});

				return widget;
			}

			case QMetaType::Double: {
				auto* widget = new QDoubleSpinBox(this);
				widget->setValue(value.toDouble());

				connect(widget, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this, key](double value) {
					m_Settings.setValue(key, value);
					emit setting_changed(key, value);
				});

				return widget;
			}

			case QMetaType::QString: {
				auto* widget = new QLineEdit(this);
				widget->setText(value.toString());

				connect(widget, &QLineEdit::textChanged, this, [this, key](const QString& value) {
					m_Settings.setValue(key, value);
					emit setting_changed(key, value);
				});

				return widget;
			}
			default:
				return nullptr;
		}
	}

	auto SettingsMenu::create_widget(const QString& key, const QStringList& combo, const QString& value) -> QWidget* {
		auto* widget = new QComboBox(this);
		widget->addItems(combo);

		widget->setCurrentText(value);

		connect(widget, &QComboBox::currentTextChanged, this, [this, key](const QString& value) {
			m_Settings.setValue(key, value);
			emit setting_changed(key, value);
		});

		return widget;
	}

	auto SettingsMenu::key(const QString& name) const -> const QString {
		return m_Name + "/" + name;
	}

} // namespace aby::eng::editor

namespace aby::eng::editor {

	ConsoleSettings::ConsoleSettings(QWidget* parent) :
	    SettingsMenu("Console", parent) {
		add("Theme",
		    QStringList{
		        "DarkPastels",
		        "BlackOnLightYellow",
		        "BlackOnRandomLight",
		        "BlackOnWhite",
		        "BreezeModified",
		        "Falcon",
		        "Falcon",
		        "GreenOnBlack",
		        "Linux",
		        "Nord",
		        "Solarized",
		        "SolarizedLight",
		        "Tango",
		        "Ubuntu",
		        "WhiteOnBlack" },
		    "The color scheme used by the console");

		add("FontSize", 12, "The font size used by the console");

		m_Layout->setRowStretch(m_Layout->rowCount(), 1);
	}

} // namespace aby::eng::editor
