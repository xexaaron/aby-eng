#include "core/ecs/systems/lifecycle-system.hpp"

#include "core/ecs/component.hpp"
#include "core/ecs/components/lifecycle-component.hpp"
#include "core/ecs/system.hpp"

#include <memory>

namespace aby::eng::ecs {

	auto LifecycleSystem::create(entt::registry& registry) -> ref<LifecycleSystem> {
		return std::make_shared<LifecycleSystem>(registry);
	}

	LifecycleSystem::LifecycleSystem(entt::registry& registry) :
	    System(registry) {
	}

	auto LifecycleSystem::on_create() -> void {
		auto entities = m_Registry->view<LifecycleComponent>();
		for (const auto entity : entities) {
			auto& lifecycle = m_Registry->get<LifecycleComponent>(entity);
			if (lifecycle.on_create)
				lifecycle.on_create(entity);
		}
	}

	auto LifecycleSystem::on_tick(const Time& deltatime) -> void {
		auto entities = m_Registry->view<LifecycleComponent>();
		for (const auto entity : entities) {
			auto& lifecycle = m_Registry->get<LifecycleComponent>(entity);
			if (lifecycle.on_tick)
				lifecycle.on_tick(entity, deltatime);
		}
	}

	auto LifecycleSystem::on_destroy() -> void {
		auto entities = m_Registry->view<LifecycleComponent>();
		for (const auto entity : entities) {
			auto& lifecycle = m_Registry->get<LifecycleComponent>(entity);
			if (lifecycle.on_destroy)
				lifecycle.on_destroy(entity);
		}
	}

} // namespace aby::eng::ecs
