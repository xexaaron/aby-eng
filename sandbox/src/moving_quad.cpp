#include "moving_quad.hpp"

namespace aby::eng::sandbox {

	auto MovingQuad::on_create() -> void {
	}

	auto MovingQuad::on_render() -> void {
		eng::Renderer2D::quad(m_QuadTransform);
	}

	auto MovingQuad::on_tick(const Time& dt) -> void {
		const auto [w, h]  = App::window()->size();
		const float speed  = 100.f; // pixels / second
		const float offset = speed * dt.sec();

		if (m_Direction) {
			m_QuadTransform.pos.x += offset;

			if (m_QuadTransform.pos.x + m_QuadTransform.size.x >= w) {
				m_QuadTransform.pos.x = w - m_QuadTransform.size.x;
				m_Direction           = false;
			}
		} else {
			m_QuadTransform.pos.x -= offset;

			if (m_QuadTransform.pos.x <= 0.f) {
				m_QuadTransform.pos.x = 0.f;
				m_Direction           = true;
			}
		}
	}

} // namespace aby::eng::sandbox
