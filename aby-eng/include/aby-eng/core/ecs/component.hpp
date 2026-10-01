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

	/**
	* Base component class that every component should inherit from.
	* Descries the static interface that all components should implement.
	*/
	struct Component {
		using type_list = detail::ComponentList;

		/**
        * Get the class name of a component
        * @note
		*
		* 	 Every component must have this static function defined.
        */
		static auto name() -> std::string_view {
			return "Component";
		}
		/**
        * Check if the component should be hidden from tree and property views
        * @note
		*
		* 	 Every component must have this static function defined
        */
		static auto hidden() -> bool {
			return true;
		}
	};

} // namespace aby::eng::ecs
