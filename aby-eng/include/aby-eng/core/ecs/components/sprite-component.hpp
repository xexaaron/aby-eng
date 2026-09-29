#pragma once

#include "core/ecs/component.hpp"
#include "core/ecs/components/component.inl"
#include "core/ecs/property.hpp"

namespace aby::eng::ecs {

	struct SpriteComponent : public Component {
		ABY_ENG_COMPONENT("Sprite", ABY_ENG_COMPONENT_SHOW);

		property<"Color", glm::fvec4, EProperty::visible> color   = { 1.f, 1.f, 1.f, 1.f };
		property<"UV Min", glm::fvec2, EProperty::visible> uv_min = { 0.f, 0.f };
		property<"UV Max", glm::fvec2, EProperty::visible> uv_max = { 0.f, 0.f };
		property<"Texture", uint32_t, EProperty::visible> texture = 0;

		ABY_ENG_PROPERTIES(color, uv_min, uv_max, texture);
	};

} // namespace aby::eng::ecs
