#pragma once

#pragma once

#include "core/app.hpp"

namespace aby::eng {

	/**
     * @brief Define your entry point class and then in your main function call EntryPoint::set<MyEntryClass>(...); 
     *        Your EntryPoint constructor must accept (int argc, char** argv) as parameters.
     */
	class ABY_API EntryPoint {
	public:
		template <typename T>
		requires(std::derived_from<T, EntryPoint>)
		static auto set(i32 argc, char** argv) -> i32 {
			m_EntryPoint = std::make_shared<T>(argc, argv);
			return m_EntryPoint->init(argc, argv);
		}
		static auto get() -> ref<EntryPoint>;

		/**
		 * @brief Called during app initialization
		 * @return A filled out app info structure reference
		 * @note The returned reference must remain valid for the lifetime of the application.
		 */
		virtual auto init() -> AppInfo& = 0;
		/**
		 * @brief Called after systems have been initialized at the start of App::run()
		 * 		  This is where you should be adding objects to the application
		 */
		virtual auto on_exec() -> void = 0;
		/**
		 * @brief Called after the window/render loop has exited before app deinitalization
		 */
		virtual auto on_exit() -> void = 0;
	private:
		auto init(int argc, char** argv) -> i32;
	private:
		static inline ref<EntryPoint> m_EntryPoint = nullptr;
	};

} // namespace aby::eng
