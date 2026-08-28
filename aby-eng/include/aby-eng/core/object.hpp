#pragma once
#include "misc/time.hpp"
#include "misc/uuid.hpp"

#include <aby-win/event.hpp>

namespace aby::eng {

	class ABY_API Object {
	public:
		virtual auto on_create() -> void;
		virtual auto on_tick(const Time& deltatime) -> void;
		virtual auto on_render() -> void;
		virtual auto on_event(win::Event& event) -> bool;
		virtual auto on_destroy() -> void;
		auto uuid() const -> UUID;
	private:
		UUID m_ID;
	};

} // namespace aby::eng
