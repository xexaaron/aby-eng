#pragma once

#include "core/ecs/system.hpp"
#include "core/object.hpp"
#include "entt/entity/fwd.hpp"

namespace aby::eng::ecs {

	class LifecycleSystem : public System {
		ABY_OBJECT_CLASS(LifecycleSystem);
	public:
		static auto create(entt::registry& registry) -> ref<LifecycleSystem>;
		LifecycleSystem(entt::registry& registry);

		auto on_create() -> void override;
		auto on_tick(const Time& deltatime) -> void override;
		auto on_destroy() -> void override;
	};

} // namespace aby::eng::ecs
