#pragma once
#include "common.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <aby-win/window.hpp>
#include <string>

namespace aby::eng {

	struct AppInfo {
		std::string name = "";
		int argc         = 0;
		char** argv      = nullptr;
	};

	class ABY_API App {
	public:
		static auto init(const AppInfo& info) -> bool;
		static auto get() -> App&;
		~App();

		auto run() -> void;
	protected:
		App(const AppInfo& info);
	private:
		unique<win::Window> m_Window;
		rhi::Context* m_Context;
	};

} // namespace aby::eng
