#include "core/object.hpp"

namespace aby::eng {

	auto Object::on_create() -> void {
	}

	auto Object::on_tick(const Time& deltatime) -> void {
	}

	auto Object::on_render() -> void {
	}

	auto Object::on_event(win::Event& event) -> bool {
		return false;
	}

	auto Object::on_destroy() -> void {
	}

	auto Object::uuid() const -> UUID {
		return m_ID;
	}

	auto Object::operator==(Object& other) -> bool {
		return m_ID == other.m_ID;
	}

} // namespace aby::eng
