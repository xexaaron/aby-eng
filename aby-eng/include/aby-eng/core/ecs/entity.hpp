#pragma once
#include "core/app.hpp"
#include "core/ecs/component.hpp"
#include "core/object.hpp"
#include "log.hpp"
#include "misc/meta.hpp"

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <type_traits>

namespace aby::eng::ecs {

	class Entity {
	public:
		/**
		* @brief Create a new entity and register it
		*/
		Entity();
		/**
		* @brief Only used for copying the id of an entity
		*/
		Entity(entt::entity id);
		/**
		* @brief Only used for copying the id of an entity
		*/
		Entity(const Entity& other);
		/**
		* @brief Sets the moved from entities id to entt::null
		*/
		Entity(Entity&& other);

		/**
		* @brief Attach a component to an entity
		* @tparam T the component type
		* @param component a component
		* @return the new component 
		*/
		template <typename T>
		requires(detail::CComponent<T>)
		auto add(const T& component) -> T&;

		/**
		* @brief Attach a component to an entity
		* @tparam T default constructible component type
		* @return the new component
		*/
		template <typename T>
		requires(detail::CComponent<T> && std::is_default_constructible_v<T>)
		auto add() -> T&;

		/**
		* @brief Attach a component to an entity
		* @tparam T the component type
		* @tparam Args the component constructor arg types
		* @param args the component constructor args
		* @return the new component
		*/
		template <typename T, typename... Args>
		requires(detail::CComponent<T> && (std::is_constructible_v<T, Args...> || meta::CAggregateInitializable<T, Component, Args...>))
		auto emplace(Args&&... args) -> T&;

		/**
		* @brief Check if the entity has this component(s)
		* @tparam ...Ts the component type(s)
		*/
		template <typename... Ts>
		requires((detail::CComponent<Ts> && ...))
		auto has() const -> bool;

		/**
		* @brief Get the component(s) belonging to this entity.
		* @tparam Ts The component type(s).
		* @return T& for a single component, or std::tuple<Ts&...> for multiple components.
		*/
		template <typename... Ts>
		requires((detail::CComponent<Ts> && ...))
		auto get() const -> std::conditional_t<sizeof...(Ts) == 1, std::tuple_element_t<0, std::tuple<Ts...>>&, std::tuple<Ts&...>>;

		/**
		* @brief Create a new entity and copy all of its component
		*/
		auto clone() const -> Entity;

		operator entt::entity() const;
		auto operator==(const Entity& other) const -> bool;
		auto operator!=(const Entity& other) const -> bool;
	private:
		entt::entity m_ID;
	};

} // namespace aby::eng::ecs

namespace aby::eng::ecs {

	template <typename T>
	requires(detail::CComponent<T>)
	auto Entity::add(const T& component) -> T& {
		auto& reg = App::entity_registry();
		return reg.emplace<T>(m_ID, component);
	}

	template <typename T>
	requires(detail::CComponent<T> && std::is_default_constructible_v<T>)
	auto Entity::add() -> T& {
		auto& reg = App::entity_registry();
		return reg.emplace<T>(m_ID);
	}

	template <typename T, typename... Args>
	requires(detail::CComponent<T> && (std::is_constructible_v<T, Args...> || meta::CAggregateInitializable<T, Component, Args...>))
	auto Entity::emplace(Args&&... args) -> T& {
		auto& reg = App::entity_registry();

		// its aggregate initializable but not constructible via args
		if constexpr (!std::is_constructible_v<T, Args...> && meta::CAggregateInitializable<T, Component, Args...>) {
			return reg.emplace<T>(m_ID, Component{}, std::forward<Args>(args)...);
		} else {
			return reg.emplace<T>(m_ID, std::forward<Args>(args)...);
		}
	}

	template <typename... Ts>
	requires((detail::CComponent<Ts> && ...))
	auto Entity::has() const -> bool {
		auto& reg = App::entity_registry();
		return reg.all_of<Ts...>(m_ID);
	}

	template <typename... Ts>
	requires((detail::CComponent<Ts> && ...))
	auto Entity::get() const -> std::conditional_t<sizeof...(Ts) == 1, std::tuple_element_t<0, std::tuple<Ts...>>&, std::tuple<Ts&...>> {
		expect(this->has<Ts...>(), "The entity does not have this component(s)");
		auto& reg = App::entity_registry();
		return reg.get<Ts...>(m_ID);
	}

} // namespace aby::eng::ecs

