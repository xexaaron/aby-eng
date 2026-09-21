#pragma once
#include <QDockWidget>

namespace aby::eng::editor {

	auto operator""_q(const char* s, std::size_t n) -> QString;

	template <typename T, typename... Args>
	auto docked(QWidget* parent, const std::string& name, Args&&... args) -> std::pair<QDockWidget*, T*> {
		auto* widget      = new T(std::forward<Args>(args)...);
		auto* widget_dock = new QDockWidget(QString::fromStdString(name), parent);
		widget_dock->setWidget(widget);
		return { widget_dock, widget };
	}

} // namespace aby::eng::editor
