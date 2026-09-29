#include "core/ecs/entity.hpp"

#include "core/app.hpp"
#include "log.hpp"

namespace aby::eng::ecs {

	Entity::Entity() {
		auto& reg = App::entity_registry();
		m_ID      = reg.create();
		App::add_entity(m_ID);
	}

	Entity::Entity(entt::entity id) :
	    m_ID(id) {
	}

	Entity::Entity(const Entity& other) :
	    m_ID(other.m_ID) {
	}

	Entity::Entity(Entity&& other) :
	    m_ID(std::exchange(other.m_ID, entt::null)) {
	}

	auto Entity::clone() const -> Entity {
		auto& reg = App::entity_registry();

		const auto entity = reg.create();

		for (auto&& [type, storage] : reg.storage()) {
			if (storage.contains(m_ID)) {
				storage.push(entity, storage.value(m_ID));
			}
		}

		App::add_entity(entity);

		return Entity{ entity };
	}

	Entity::operator entt::entity() const {
		return m_ID;
	}

	auto Entity::operator==(const Entity& other) const -> bool {
		return m_ID == other.m_ID;
	}

	auto Entity::operator!=(const Entity& other) const -> bool {
		return m_ID != other.m_ID;
	}

} // namespace aby::eng::ecs
