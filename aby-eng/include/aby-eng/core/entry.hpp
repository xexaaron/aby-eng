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
		static auto set(i32 argc, char** argv) -> void {
			m_EntryPoint = std::make_shared<T>(argc, argv);
			m_EntryPoint->init(argc, argv);
		}
		static auto get() -> ref<EntryPoint>;

		virtual auto init() -> AppInfo         = 0;
		virtual auto on_exec(App& app) -> void = 0;
		virtual auto on_exit() -> void         = 0;
	private:
		auto init(int argc, char** argv) -> i32;
	private:
		static inline ref<EntryPoint> m_EntryPoint = nullptr;
	};

} // namespace aby::eng
