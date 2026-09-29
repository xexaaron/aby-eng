#pragma once

#include "core/ecs/property.hpp"
#include "misc/meta.hpp"

#include <format>
#include <string_view>
#include <typeinfo>

#define ABY_ENG_COMPONENT_HIDE true
#define ABY_ENG_COMPONENT_SHOW false

/**
* @brief Define common component functions
* @param Name the class name of the component
* @param Hidden the tree/property visibility of the component
*/
#define ABY_ENG_COMPONENT(Name, Hidden)      \
	static auto name() -> std::string_view { \
		return Name;                         \
	}                                        \
	static auto hidden() -> bool {           \
		return Hidden;                       \
	}

namespace aby::eng::ecs {

	class Component;
	class TransformComponent;
	class SpriteComponent;
	class LifecycleComponent;

	namespace detail {

		template <typename T>
		concept CComponent = std::derived_from<T, Component> &&
		                     std::is_copy_constructible_v<T> &&
		                     std::is_copy_assignable_v<T> &&
		                     requires {
			                     { T::name() } -> std::same_as<std::string_view>;
			                     { T::hidden() } -> std::same_as<bool>;
		                     };

		using ComponentList = meta::type_list<
		    TransformComponent,
		    SpriteComponent,
		    LifecycleComponent>;

	} // namespace detail

} // namespace aby::eng::ecs

