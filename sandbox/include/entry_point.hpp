#pragma once

#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-eng/core/renderer.hpp>
#include <aby-eng/misc/font.hpp>
#include <aby-eng/ui/button.hpp>
#include <aby-eng/ui/containers/aspectratiobox.hpp>
#include <aby-eng/ui/containers/canvas.hpp>
#include <aby-eng/ui/containers/container.hpp>
#include <aby-eng/ui/containers/grid.hpp>
#include <aby-eng/ui/image.hpp>
#include <aby-eng/ui/window.hpp>

namespace aby::eng::sandbox {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv);

		auto on_exec() -> void override;
		auto on_exit() -> void override;
	private:
		AppInfo m_AppInfo;
		FontPtr m_Font;
	};

} // namespace aby::eng::sandbox
