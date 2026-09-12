#pragma once

#include "core/app.hpp"

#include <argparse/argparse.hpp>

namespace aby::eng {

	/**
     * @brief Define your entry point class and then in your main function call EntryPoint::set<MyEntryClass>(...); 
     */
	class ABY_API EntryPoint {
	public:
		EntryPoint(const AppInfo& app_info);

		/**
		* @brief Set the global entry point
		* @tparam EntryPoint derived class
		* @param argc the argument count supplied by the main function
		* @param argv the arguments supplied by the main function
		* @return the application exit code  
		*/
		template <typename T>
		requires(std::derived_from<T, EntryPoint>)
		static auto set(i32 argc, char** argv) -> i32 {
			m_EntryPoint = std::make_shared<T>(argc, argv);
			return m_EntryPoint->init(argc, argv);
		}

		/**
		* @brief Get the globally set entry point
		*/
		static auto get() -> ref<EntryPoint>;

		/**
		* @brief Callback to have the application register arguments for parsing.
		* @param parser The command line parser to add arguments to.
		* @note Use @c parser.store_into(...) to store parsed values.
		* @note Optionally you can set the epilog or description for the application.
		* @warning Do not call @c parse or @c parse_known_args on the parser. 
		*/
		virtual auto on_cmdl(argparse::ArgumentParser& parser) -> void;

		/**
		 * @brief Callback to add app data such as objects or ui components.
		 * @note Called after systems have been initialized at the start of App::run()
		 */
		virtual auto on_exec() -> void = 0;

		/**
		* @brief Callback for after all objects have been created 
		* @note Called after systems have been initialized right before the main loop. 
		* 		Different from @c on_exec which is called before @c Object::on_create initialization 
		*/
		virtual auto on_create() -> void;

		/**
		 * @brief Callback for when the application exits
		 * @note Called after the window/render loop has exited before app deinitalization
		 */
		virtual auto on_exit() -> void = 0;
	private:
		auto init(int argc, char** argv) -> i32;
	private:
		static inline ref<EntryPoint> m_EntryPoint = nullptr;
		AppInfo m_AppInfo;
	};

} // namespace aby::eng
