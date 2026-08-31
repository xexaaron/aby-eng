#include "ui/containers/aspectratiobox.hpp"

#include "core/app.hpp"

namespace aby::eng::ui {

	AspectRatioBox::AspectRatioBox(const Transform2D& transform) :
	    Element(transform) {
		auto [x, y]    = App::window()->size();
		m_WindowSize.x = x;
		m_WindowSize.y = y;
	}

	auto AspectRatioBox::create(const Transform2D& transform) -> ref<AspectRatioBox> {
		return std::make_shared<AspectRatioBox>(transform);
	}

	auto AspectRatioBox::on_event(win::Event& event) -> bool {
		win::EventDispatcher dsp(event);
		dsp.dispatch(&AspectRatioBox::on_window_resize, this);
		return Element::on_event(event);
	}

	auto AspectRatioBox::on_window_resize(win::WindowResizedEvent& event) -> bool {
		const glm::fvec2 new_size{
			static_cast<float>(event->width),
			static_cast<float>(event->height)
		};

		const glm::fvec2 scale = new_size / m_WindowSize;

		m_WindowSize = new_size;

		m_Transform.scale_by(scale);

		for (auto& child : m_Children) {
			child->set_scale(child->scale() * scale);
		}

		return false;
	}

} // namespace aby::eng::ui
