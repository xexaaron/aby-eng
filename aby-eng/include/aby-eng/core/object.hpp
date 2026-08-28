#pragma once
#include "core/app.hpp"
#include "misc/uuid.hpp"
namespace aby::eng {

	class Object {
	public:
		virtual auto on_create() -> void;
		virtual auto on_tick(float deltatime) -> void;
		virtual auto on_render() -> void;
		virtual auto on_event(win::Event& event) -> bool;
		virtual auto on_destroy() -> void;
		auto uuid() const -> UUID;
	private:
		UUID m_ID;
	};

} // namespace aby::eng
