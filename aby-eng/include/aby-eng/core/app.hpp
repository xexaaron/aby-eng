#pragma once
#include "common.hpp"
#include "core/object.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <aby-win/window.hpp>
#include <entt/entt.hpp>
#include <optional>
#include <string>
#include <vector>

namespace aby::eng {

	namespace ecs {

		class Entity;

	}

	struct AppInfo {
		std::string name    = "";
		std::string version = "1.0";
		i32 argc            = 0;
		char** argv         = nullptr;
		win::Config win_cfg = {};
	};

	enum class EAppState {
		init,
		running,
		deinit,
	};

	struct EngineArgs {
		bool render_doc = false;
	};

	/**
	* Core application class
	* @note
	* 	
	* 	 The entry point will call @c App::run
	*/
	class ABY_API App {
	public:
		/**
		* Run the application, create objects, initialize entity systems
		*/
		static auto run() -> void;
		/**
		* Exit the application and cleanup all resources
		*/
		static auto exit() -> void;
		/**
		* Get the application viewport window
		*/
		static auto window() -> win::Window*;
	protected:
		App(const AppInfo& info);
	private:
		/**
		* Initializes all rendering systems and windowing libraries
		*/
		static auto init(const AppInfo& info) -> bool;
		/**
		* Cleanup all rendering systems and windows
		*/
		static auto deinit() -> void;
		/**
		* Parses engine arguments and application arguments
		*/
		static auto parse_args(const AppInfo& info) -> EngineArgs;
		friend class EntryPoint;
	private:
		/**
		* Add an object class to the application
		* @param[in] object
		*/
		static auto add_obj(ref<Object> object) -> void;
		template <typename T, typename... Args>
		requires(CObject<T>)
		friend auto create(Args&&... args) -> ref<T>;
	private:
		/**
		* Get the entity registry
		*/
		static auto entity_registry() -> entt::registry&;
		/**
		* Add an entity to the application
		*/
		static auto add_entity(entt::entity entity) -> void;
		friend class ecs::Entity;
	private:
		static inline unique<win::Window> m_Window = nullptr;
		static inline rhi::Context* m_Context      = nullptr;
		static inline std::vector<ref<Object>> m_Objects;
		static inline EAppState m_State = EAppState::init;
		static inline entt::registry m_EntityRegistry;
	};

	template <typename T, typename... Args>
	requires(CObject<T>)
	static auto create(Args&&... args) -> ref<T> {
		ref<T> obj = T::create(std::forward<Args>(args)...);
		App::add_obj(obj);
		return obj;
	}

} // namespace aby::eng

namespace aby::eng::detail {

	class RHILoggerInterface : public rhi::ILogger {
	public:
		auto log(rhi::ELogLevel level, const std::string& msg) -> void override;
	private:
	};

	class WINLoggerInterface : public win::ILogger {
	public:
		auto log(win::ELogLevel level, const std::string& msg) -> void override;
	private:
	};

} // namespace aby::eng::detail
