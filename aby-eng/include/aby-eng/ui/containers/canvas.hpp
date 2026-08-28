#pragma once
#include "ui/element.hpp"

namespace aby::eng::ui {

	class ABY_API Canvas : public Element {
	public:
		Canvas();
		Canvas(const glm::fvec4& color);

		auto on_render() -> void override;
		auto on_event(win::Event& event) -> bool override;

		auto set_color(const glm::fvec4& color);
		auto set_opacity(float opacity);

		auto color() const -> const glm::fvec4&;
		auto opacity() const -> float;
	protected:
		auto on_window_resize(win::WindowResizedEvent& event) -> bool;
	protected:
		Material2D m_Material;
	};

} // namespace aby::eng::ui
