#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-eng/core/renderer.hpp>
#include <aby-eng/log.hpp>
#include <aby-eng/misc/font.hpp>
#include <aby-eng/misc/utf8.hpp>
#include <aby-eng/ui/button.hpp>
#include <aby-eng/ui/containers/aspectratiobox.hpp>
#include <aby-eng/ui/containers/canvas.hpp>
#include <aby-eng/ui/containers/container.hpp>
#include <aby-eng/ui/containers/grid.hpp>
#include <aby-eng/ui/image.hpp>
#include <aby-eng/ui/window.hpp>
#include <algorithm>

namespace aby::eng::editor {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv) :
		    eng::EntryPoint({ .name = "Abyss Engine",
			                  .argc = argc,
			                  .argv = argv }) {
		}

		auto on_exec() -> void {
		}

		auto on_exit() -> void {
		}
	private:
		AppInfo m_AppInfo;
	};

} // namespace aby::eng::editor

int main(int argc, char** argv) {
	using namespace aby::eng::editor;
	return EntryPoint::set<EntryPoint>(argc, argv);
}
