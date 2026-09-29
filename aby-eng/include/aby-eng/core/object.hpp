#pragma once
#include "misc/time.hpp"
#include "misc/uuid.hpp"

#include <aby-win/event.hpp>
#include <concepts>
#include <type_traits>

#define ABY_OBJECT_CLASS(Class)             \
private:                                    \
	template <typename T, typename... Args> \
	requires(CObject<T>)                    \
	friend auto ::aby::eng::create(Args&&... args)->ref<T>;

#define ABY_OBJECT_DEFAULT_CREATE(Class)  \
private:                                  \
	static auto create() -> ref<Class> {  \
		return std::make_shared<Class>(); \
	}

namespace aby::eng {

	class Object;

	template <typename T>
	concept CObject = (std::derived_from<T, Object> || std::is_same_v<T, Object>);

	/**
	* @brief Create an object and add it to the application
	* @tparam T the object type (derived from Object base class)
	* @tparam Args T::create argument types
	* @param args T::create arguments
	* @return ref<T> 
	* @note defined in "core/app.hpp" 
	*/
	template <typename T, typename... Args>
	requires(CObject<T>)
	static auto create(Args&&... args) -> ref<T>;

	class ABY_API Object {
		ABY_OBJECT_CLASS(Object);
		ABY_OBJECT_DEFAULT_CREATE(Object);
	public:
		virtual ~Object() = default;

		virtual auto on_create() -> void;
		virtual auto on_tick(const Time& deltatime) -> void;
		virtual auto on_render() -> void;
		virtual auto on_event(win::Event& event) -> bool;
		virtual auto on_destroy() -> void;
		auto uuid() const -> UUID;

		auto operator==(Object& other) -> bool;
	private:
		UUID m_ID;
	};

} // namespace aby::eng
