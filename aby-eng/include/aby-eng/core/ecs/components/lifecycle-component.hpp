#pragma once

#include "core/ecs/component.hpp"
#include "core/ecs/components/component.inl"
#include "core/ecs/entity.hpp"
#include "core/ecs/property.hpp"

namespace aby::eng::ecs {

	struct LifecycleComponent : public Component {
		ABY_ENG_COMPONENT("Lifecycle", ABY_ENG_COMPONENT_HIDE);

		property<"On Create", std::function<void(Entity)>, EProperty::hidden> on_create          = nullptr;
		property<"On Tick", std::function<void(Entity, const Time&)>, EProperty::hidden> on_tick = nullptr;
		property<"On Destroy", std::function<void(Entity)>, EProperty::hidden> on_destroy        = nullptr;

		ABY_ENG_PROPERTIES(on_create, on_tick, on_destroy);
	};

} // namespace aby::eng::ecs
