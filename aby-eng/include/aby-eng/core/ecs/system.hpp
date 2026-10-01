#pragma once

#include "core/ecs/entity.hpp"
#include "core/object.hpp"
#include "entt/entity/fwd.hpp"

namespace aby::eng::ecs {

	/**
	* The base system class that all systems should inherit from
	*/
	class System : public Object {
		ABY_OBJECT_CLASS(System);
	protected:
		System(entt::registry& registry);

		/// Systems are not allowed to use @c on_render
		auto on_render() -> void final override;
	protected:
		entt::registry* m_Registry;
	};

} // namespace aby::eng::ecs
