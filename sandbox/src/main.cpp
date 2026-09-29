#include "main.hpp"

#include "core/ecs/component.hpp"
#include "core/ecs/components/component.inl"
#include "core/ecs/property.hpp"
#include "misc/meta.hpp"

#include <aby-eng/core/ecs/components/lifecycle-component.hpp>
#include <aby-eng/core/ecs/components/sprite-component.hpp>
#include <aby-eng/core/ecs/components/transform-component.hpp>
#include <aby-eng/core/ecs/entity.hpp>
#include <glm/gtc/constants.hpp>

namespace aby::eng::sandbox {

	struct MoveDir : public ecs::Component {
		ABY_ENG_COMPONENT("MoveDir", ABY_ENG_COMPONENT_SHOW);

		ecs::property<"direction", float, ecs::EProperty::visible> direction = 1.f;
		ecs::property<"phase", float, ecs::EProperty::visible> phase         = 0.f;

		ABY_ENG_PROPERTIES(direction, phase);
	};

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "aby-eng-sandbox",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_exec() -> void {
		const auto [w, h] = App::window()->size();

		ecs::Entity moving_quad;
		moving_quad.add<MoveDir>();

		const glm::fvec3 size{ 50.f, 50.f, 0.f };

		auto& transform = moving_quad.emplace<ecs::TransformComponent>(
		    glm::fvec3{
		        static_cast<float>(w) * 0.5f - size.x * 0.5f,
		        static_cast<float>(h) * 0.5f - size.y * 0.5f,
		        0.f },
		    size,
		    glm::fvec3{ 1.f, 1.f, 1.f });

		auto& sprite = moving_quad.emplace<ecs::SpriteComponent>(
		    glm::fvec4{ 1.f, 1.f, 1.f, 1.f },
		    glm::fvec2{ 0.f, 0.f },
		    glm::fvec2{ 1.f, 1.f },
		    uint32_t(0));

		auto& lifecycle = moving_quad.add<ecs::LifecycleComponent>();

		lifecycle.on_tick = [](ecs::Entity entity, const Time& dt) {
			constexpr float speed  = 1.f;
			constexpr float radius = 150.f;

			auto& transform = entity.get<ecs::TransformComponent>();
			auto& movement  = entity.get<MoveDir>();

			const auto [w, h] = App::window()->size();

			movement.phase += movement.direction * speed * dt.sec();

			constexpr float tau = glm::two_pi<float>();

			if (movement.phase > tau)
				movement.phase -= tau;
			else if (movement.phase < 0.f)
				movement.phase += tau;

			const glm::fvec2 center{
				static_cast<float>(w) * 0.5f,
				static_cast<float>(h) * 0.5f
			};

			const glm::fvec2 position{
				center.x + std::cos(movement.phase) * radius,
				center.y + std::sin(movement.phase) * radius
			};

			transform.pos.x = position.x - transform.size.x * 0.5f;
			transform.pos.y = position.y - transform.size.y * 0.5f;
		};

		auto moving_quad2   = moving_quad.clone();
		auto& direction     = moving_quad2.get<MoveDir>();
		direction.phase     = glm::pi<float>();
		direction.direction = -1.f;
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::sandbox

int main(int argc, char** argv) {
	using namespace aby::eng::sandbox;
	return EntryPoint::exec<EntryPoint>(argc, argv);
}
