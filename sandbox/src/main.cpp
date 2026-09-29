#include "main.hpp"

#include "core/ecs/component.hpp"
#include "core/ecs/components/component.inl"
#include "misc/meta.hpp"

#include <aby-eng/core/ecs/components/lifecycle-component.hpp>
#include <aby-eng/core/ecs/components/sprite-component.hpp>
#include <aby-eng/core/ecs/components/transform-component.hpp>
#include <aby-eng/core/ecs/entity.hpp>

namespace aby::eng::sandbox {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "aby-eng-sandbox",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_exec() -> void {
		ecs::Entity moving_quad;

		auto& transform = moving_quad.emplace<ecs::TransformComponent>(
		    glm::fvec3{ 0.f, 0.f, 0.f },
		    glm::fvec3{ 50.f, 50.f, 0.f },
		    glm::fvec3{ 1.f, 1.f, 1.f });

		auto& sprite = moving_quad.emplace<ecs::SpriteComponent>(
		    glm::fvec4{ 1.f, 1.f, 1.f, 1.f },
		    glm::fvec2{ 0.f, 0.f },
		    glm::fvec2{ 1.f, 1.f },
		    uint32_t(0));

		auto& lifecycle = moving_quad.add<ecs::LifecycleComponent>();

		lifecycle.on_tick = [moving_quad](const Time& dt) {
			static bool direction = true;
			constexpr float speed = 100.f; // px / sec
			const float offset    = speed * dt.sec();
			const auto [w, h]     = App::window()->size();

			auto& transform = moving_quad.get<ecs::TransformComponent>();

			if (direction) {
				transform.pos.x += offset;
				if (transform.pos.x + transform.size.x >= w) {
					transform.pos.x = w - transform.size.x;
					direction       = false;
				}
			} else {
				transform.pos.x -= offset;
				if (transform.pos.x <= 0.f) {
					transform.pos.x = 0.f;
					direction       = true;
				}
			}
		};
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::sandbox

int main(int argc, char** argv) {
	using namespace aby::eng::sandbox;
	return EntryPoint::exec<EntryPoint>(argc, argv);
}
