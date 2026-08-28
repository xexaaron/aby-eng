#pragma once
#include "common.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <aby-win/window.hpp>
#include <string>

namespace aby::eng {

	struct AppInfo {
		std::string name = "";
		i32 argc         = 0;
		char** argv      = nullptr;
	};

	class ABY_API App {
	public:
		static auto run() -> void;
		static auto window() -> win::Window*;
	protected:
		App(const AppInfo& info);
	private:
		static auto init(const AppInfo& info) -> bool;
		static auto deinit() -> void;
		friend class EntryPoint;
	private:
		static inline unique<win::Window> m_Window = nullptr;
		static inline rhi::Context* m_Context      = nullptr;
	};

} // namespace aby::eng
