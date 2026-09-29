#pragma once
#include "core/ecs/components/component.inl"
#include "entt/entity/fwd.hpp"
#include "misc/meta.hpp"
#include "misc/time.hpp"
#include "misc/types.hpp"

#include <any>
#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>

namespace aby::eng::ecs {

	struct Component {
		using type_list = detail::ComponentList;

		/**
        * @brief Get the class name of a component
        * @note Every component must have this static function defined.
        */
		static auto name() -> std::string_view {
			return "Component";
		}
		/**
        * @brief Check if the component should be hidden from tree and property views
        * @note Every component must have this static function defined
        */
		static auto hidden() -> bool {
			return true;
		}
	};

} // namespace aby::eng::ecs
