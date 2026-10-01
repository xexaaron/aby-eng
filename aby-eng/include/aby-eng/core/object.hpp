#pragma once
#include "misc/time.hpp"
#include "misc/uuid.hpp"

#include <aby-win/event.hpp>
#include <concepts>
#include <type_traits>

/**
* Define the object class common properties
* @param Class the name of the class
*/
#define ABY_OBJECT_CLASS(Class)             \
private:                                    \
	template <typename T, typename... Args> \
	requires(CObject<T>)                    \
	friend auto ::aby::eng::create(Args&&... args)->ref<T>;

/**
* Mark the object as default constructible and automatically make the required
* create function 
* @param Class the name of the class
*/
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
	* Create an object and add it to the application
	* @tparam T the object type (derived from Object base class)
	* @tparam Args T::create argument types
	* @param args T::create arguments
	* @return ref<T> 
	* @note
	*
	* 	defined in "core/app.hpp" 
	*/
	template <typename T, typename... Args>
	requires(CObject<T>)
	static auto create(Args&&... args) -> ref<T>;

	/**
	* The core object class to execute functionality during the application lifecycle
	*/
	class ABY_API Object {
		ABY_OBJECT_CLASS(Object);
		ABY_OBJECT_DEFAULT_CREATE(Object);
	public:
		virtual ~Object() = default;

		/**
		* Called either when the app starts running or when created via @code create<...>(...) @endcode
		*/
		virtual auto on_create() -> void;

		/**
		* Called every application tick
		* @param[in] deltatime the time between the last frame and the current frame
		*/
		virtual auto on_tick(const Time& deltatime) -> void;

		/**
		* Called when the renderer is accepting commands
		*/
		virtual auto on_render() -> void;

		/**
		* Called when the window dispatches an event
		* @param event the window event
		* @return true to stop propagating the event to other objects
		*/
		virtual auto on_event(win::Event& event) -> bool;

		/** 
		* Called before the app shutsdown
		*/
		virtual auto on_destroy() -> void;

		/**
		* Get the uuid of this object
		*/
		auto uuid() const -> UUID;

		auto operator==(Object& other) -> bool;
	private:
		UUID m_ID;
	};

} // namespace aby::eng
