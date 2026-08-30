#include <aby-eng/core/app.hpp>
#include <aby-eng/core/entry.hpp>
#include <aby-eng/core/renderer.hpp>
#include <aby-eng/ui/containers/canvas.hpp>
#include <aby-eng/ui/containers/container.hpp>
#include <aby-eng/ui/containers/grid.hpp>

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
			auto canvas = ui::Canvas::create(glm::fvec4{ 1.f, 0.f, 0.f, 0.25f });

			auto primary_container = ui::HContainer::create(
			    Transform2D({ 0, 0 },
			                { 300, 300 }, 1.f),
			    ui::EDirection::forward,
			    2.f,
			    15.f,
			    ui::Border(2.f, glm::fvec4{ 0.f, 1.f, 0.f, 1.f }),
			    ui::EStretch::fill);

			for (size_t i = 0; i < 4; i++) {
				glm::fvec4 color;
				switch (i) {
					case 0:
						color = glm::fvec4(1.f, 0.f, 0.f, 1.f);
						break;
					case 1:
						color = glm::fvec4(0.f, 0.f, 1.f, 1.f);
						break;
					case 2:
						color = glm::fvec4(1.f, 0.f, 1.f, 1.f);
						break;
					case 3:
						color = glm::fvec4(1.f, 1.f, 0.f, 1.f);
						break;
				}

				auto vcontainer = ui::VContainer::create(
				    Transform2D({ 0, 0 }, { 75, 300 }, 1.f),
				    ui::EDirection::forward,
				    1.f,
				    0.f,
				    ui::Border(2.f, color),
				    ui::EStretch::fill);

				for (size_t j = 0; j < 4; j++) {
					glm::fvec4 color;
					switch (j) {
						case 0:
							color = glm::fvec4(1.f, 0.f, 0.f, 1.f);
							break;
						case 1:
							color = glm::fvec4(0.f, 0.f, 1.f, 1.f);
							break;
						case 2:
							color = glm::fvec4(1.f, 0.f, 1.f, 1.f);
							break;
						case 3:
							color = glm::fvec4(1.f, 1.f, 0.f, 1.f);
							break;
					}

					auto hcontainer = ui::HContainer::create(
					    Transform2D({ 0, 0 }, { 75, 75 }, 1.f),
					    ui::EDirection::forward,
					    0.f,
					    15.f,
					    ui::Border(2.f, color),
					    ui::EStretch::fill);

					vcontainer->add_child(hcontainer);
				}

				primary_container->add_child(vcontainer);
			}

			canvas->add_child(primary_container);

			auto grid = ui::Grid::create(Transform2D({ 300, 0 }, { 300, 300 }, 1.f), 4, 4);
			grid->set_border(2.f, { 0.15f, 0.15f, 0.15f, 1.f });

			canvas->add_child(grid);

			App::add_obj(canvas);
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
