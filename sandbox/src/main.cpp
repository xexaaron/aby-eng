#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-eng/core/renderer.hpp>

namespace aby::eng::sandbox {

	class TestObject : public eng::Object {
	public:
		auto on_render() -> void override {
			eng::Renderer2D::quad(m_QuadTransform);
		}

		auto on_tick(const Time& dt) -> void override {
			const auto [w, h]  = App::window()->size();
			const float speed  = 100.f; // pixels / second
			const float offset = speed * dt.sec();

			if (m_Direction) {
				m_QuadTransform.pos.x += offset;

				if (m_QuadTransform.pos.x + m_QuadTransform.size.x >= w) {
					m_QuadTransform.pos.x = w - m_QuadTransform.size.x;
					m_Direction           = false;
				}
			} else {
				m_QuadTransform.pos.x -= offset;

				if (m_QuadTransform.pos.x <= 0.f) {
					m_QuadTransform.pos.x = 0.f;
					m_Direction           = true;
				}
			}
		}
	private:
		bool m_Direction                 = true;
		eng::Transform2D m_QuadTransform = eng::Transform2D({ 0, 0 }, { 200, 200 }, 1.f);
	};

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
			App::add_obj(std::make_shared<TestObject>());
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
