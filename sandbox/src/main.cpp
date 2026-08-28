#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-eng/core/renderer.hpp>

namespace aby::eng::sandbox {

	class EntryPoint final : public eng::EntryPoint {
	public:
		EntryPoint(int argc, char** argv) :
		    m_AppInfo{
			    .name = "aby-eng-sandbox",
			    .argc = argc,
			    .argv = argv
		    } {
		}

		auto init() -> AppInfo& {
			return m_AppInfo;
		}

		auto on_exec() -> void {
			eng::Renderer2D::quad(eng::Transform2D(
			                          { 0, 0 },
			                          { 200, 200 },
			                          1.f),
			                      eng::Material2D());
		}

		auto on_exit() -> void {
		}
	private:
		AppInfo m_AppInfo;
	};

} // namespace aby::eng::sandbox

int main(int argc, char** argv) {
	using namespace aby::eng::sandbox;
	return EntryPoint::set<EntryPoint>(argc, argv);
}
