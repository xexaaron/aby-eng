#pragma once
#include "core/ecs/component.hpp"
#include "core/ecs/components/component.inl"
#include "core/ecs/property.hpp"
#include "misc/meta.hpp"

namespace aby::eng::ecs {

	struct TransformComponent : public Component {
		ABY_ENG_COMPONENT("Transform", ABY_ENG_COMPONENT_SHOW);

		property<"Position", glm::fvec3, EProperty::visible> pos = { 0.f, 0.f, 0.f };
		property<"Size", glm::fvec3, EProperty::visible> size    = { 0.f, 0.f, 0.f };
		property<"Scale", glm::fvec3, EProperty::visible> scale  = { 0.f, 0.f, 0.f };

		ABY_ENG_PROPERTIES(pos, size, scale);
	};

} // namespace aby::eng::ecs
