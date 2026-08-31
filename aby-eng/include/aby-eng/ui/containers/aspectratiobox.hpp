#pragma once

#include "ui/element.hpp"

namespace aby::eng::ui {

	/// @brief Aspect ratio box class
	///        Scales elements based on the window size
	class ABY_API AspectRatioBox : public Element {
	public:
		AspectRatioBox(const Transform2D& transform);

		static auto create(const Transform2D& transform) -> ref<AspectRatioBox>;

		auto on_event(win::Event& event) -> bool override;
	protected:
		auto on_window_resize(win::WindowResizedEvent& event) -> bool;
	private:
		glm::fvec2 m_WindowSize;
	};

} // namespace aby::eng::ui
