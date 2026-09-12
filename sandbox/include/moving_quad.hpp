#pragma once

#include <aby-eng/core/app.hpp>
#include <aby-eng/core/object.hpp>
#include <aby-eng/core/renderer.hpp>

namespace aby::eng::sandbox {

	class MovingQuad : public eng::Object {
	public:
		auto on_create() -> void override;
		auto on_render() -> void override;
		auto on_tick(const Time& dt) -> void override;
	private:
		bool m_Direction                 = true;
		eng::Transform2D m_QuadTransform = eng::Transform2D({ 0, 0 }, { 200, 200 }, 1.f);
		FontPtr m_EmojiFont              = nullptr;
	};

} // namespace aby::eng::sandbox
