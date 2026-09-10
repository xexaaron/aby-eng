#pragma once
#include "common-enums.hpp"
#include "common.hpp"
#include "window.hpp"

#include <algorithm>
#include <cstddef>

namespace aby::eng::detail {

	template <size_t N>
	struct TemplateString {
		constexpr TemplateString(const char (&str)[N]) {
			std::copy_n(str, N, buf);
		}

		char buf[N]{};
	};

} // namespace aby::eng::detail

namespace aby::eng {

	class Registry {
	public:
		/**
		* @brief Get a registry variable by its name
		* @tparam "win-backend" 		The aby::win window backend
		* @return "win-backend" 		 -> aby::win::EWindow
		* @tparam "rhi-backend" 		The aby::rhi renderer backend
		* @return "rhi-backend" 		 -> aby::rhi::ERenderer
		* @tparam "win-render-backend" 	The aby::win renderer backend
		* @return "win-renderer-backend" -> aby::win::ERenderBackend
		*/
		template <detail::TemplateString str>
		static constexpr auto get();

		/**
		* @brief Get a registry variable by its name with a control flag
		* @tparam "rhi-window-backend":true  The aby::rhi window backend with render doc compatibility
		* @tparam "rhi-window-backend":false The aby::rhi window backend without render doc compatibility
		* @return "rhi-window-backend" -> aby::rhi::EWindow
		*/
		template <detail::TemplateString str, bool ex_flag>
		static constexpr auto get();

		static auto get(std::string_view str) -> int64_t;
		static auto set(std::string_view str, int64_t value) -> void;
	};

	template <>
	constexpr auto Registry::get<"win-backend">() {
		return win::EWindow::glfw;
	}

	template <>
	constexpr auto Registry::get<"rhi-backend">() {
		return rhi::ERenderer::vulkan;
	}

	template <>
	constexpr auto Registry::get<"rhi-window-backend", false>() {
#ifdef _WIN32
		return rhi::EWindow::win32;
#elif defined(__linux__)
		return rhi::EWindow::wayland;
#elif defined(__APPLE__)
		return rhi::EWindow::metal;
#else
#	error "unsupported platform"
#endif
	}

	template <>
	constexpr auto Registry::get<"rhi-window-backend", true>() {
#ifdef _WIN32
		return rhi::EWindow::win32;
#elif defined(__linux__)
		return rhi::EWindow::x11;
#elif defined(__APPLE__)
		return rhi::EWindow::metal;
#else
#	error "unsupported platform"
#endif
	}

	template <>
	constexpr auto Registry::get<"win-render-backend">() {
		return win::ERenderBackend::vulkan;
	}

} // namespace aby::eng
