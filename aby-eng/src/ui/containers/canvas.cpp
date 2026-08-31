#include "ui/containers/canvas.hpp"

#include "core/app.hpp"
#include "core/renderer.hpp"

namespace aby::eng::ui {

	auto Canvas::create(const glm::fvec4& color) -> ref<Canvas> {
		return std::make_shared<Canvas>(color);
	}

	Canvas::Canvas() :
	    Element(eng::Transform2D({ 0, 0 }, { App::window()->width(), App::window()->height() }, 1.f)) {
	}

	Canvas::Canvas(const glm::fvec4& color) :
	    Element(eng::Transform2D({ 0, 0 }, { App::window()->width(), App::window()->height() }, 1.f)),
	    m_Material(color) {
	}

	auto Canvas::on_render() -> void {
		Renderer2D::quad(m_Transform, m_Material);
		Element::on_render();
	}

	auto Canvas::on_event(win::Event& event) -> bool {
		win::EventDispatcher dsp(event);
		dsp.dispatch(&Canvas::on_window_resize, this);
		return Element::on_event(event);
	}

	auto Canvas::on_window_resize(win::WindowResizedEvent& event) -> bool {
		m_Transform.size.x = event->width;
		m_Transform.size.y = event->height;
		return false;
	}

	auto Canvas::set_color(const glm::fvec4& color) {
		m_Material.color = color;
	}

	auto Canvas::set_opacity(float opacity) {
		m_Material.color.a = opacity;
	}

	auto Canvas::color() const -> const glm::fvec4& {
		return m_Material.color;
	}

	auto Canvas::opacity() const -> float {
		return m_Material.color.a;
	}

} // namespace aby::eng::ui
