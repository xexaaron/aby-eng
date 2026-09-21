#pragma once

#include <QDialog>
#include <QObject>
#include <QSettings>
#include <QSplitter>
#include <QStackedWidget>
#include <QTreeWidget>
#include <qboxlayout.h>
#include <qsettings.h>
#include <qvariant.h>
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

	class SettingsMenu : public QWidget {
		Q_OBJECT
	public:
		explicit SettingsMenu(const QString& name, QWidget* parent = nullptr);

		auto settings() -> QSettings&;

		/**
		* @brief Add a setting (if the setting is present in the cached values then default_value is ignored) 
		* @param[in] name The name of the setting
		* @param[in] default_value[int|double] creates a QSpinBox
		* @param[in] default_value[bool] creates a QCheckBox
		* @param[in] default_value[QString] creates a QLineEdit
		* @param[in] default_value[QStringList] creates a QComboBox with [0] being the default
		* @param[in] help message to display when hovering the setting
		*/
		auto add(const QString& name, QVariant default_value, const QString& help = "") -> void;

		template <typename T>
		auto value_or(const QString& name, T other = {}) -> T {
			return m_Settings.value(key(name), QVariant::fromValue(other)).template value<T>();
		}
	signals:
		void setting_changed(const QString& name, const QVariant& value);
	private:
		auto key(const QString& name) const -> const QString;
		auto create_widget(const QString& key, const QVariant& value) -> QWidget*;
		auto create_widget(const QString& key, const QStringList& combo, const QString& value) -> QWidget*;
	protected:
		QSettings m_Settings;
		QString m_Name;
		QGridLayout* m_Layout;
	};

	class ConsoleSettings : public SettingsMenu {
		Q_OBJECT
	public:
		explicit ConsoleSettings(QWidget* parent = nullptr);
	private:
	};

} // namespace aby::eng::editor
