#include "core/ecs/system.hpp"

#include <memory>

namespace aby::eng::ecs {

	System::System(entt::registry& registry) :
	    m_Registry(&registry) {
	}

	auto System::on_render() -> void {
	}

} // namespace aby::eng::ecs
