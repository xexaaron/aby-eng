#pragma once
#include "common.hpp"
#include "core/object.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <aby-win/window.hpp>
#include <string>
#include <vector>

namespace aby::eng {

	struct AppInfo {
		std::string name    = "";
		std::string version = "1.0";
		i32 argc            = 0;
		char** argv         = nullptr;
	};

	enum class EAppState {
		init,
		running,
		deinit,
	};

	class ABY_API App {
	public:
		static auto run() -> void;
		static auto window() -> win::Window*;
		static auto add_obj(ref<Object> object) -> void;
	protected:
		App(const AppInfo& info);
	private:
		static auto init(const AppInfo& info) -> bool;
		static auto deinit() -> void;
		friend class EntryPoint;
	private:
		static inline unique<win::Window> m_Window = nullptr;
		static inline rhi::Context* m_Context      = nullptr;
		static inline std::vector<ref<Object>> m_Objects;
		static inline EAppState m_State = EAppState::init;
	};

} // namespace aby::eng
