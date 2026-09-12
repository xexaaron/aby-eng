#include "entry_point.hpp"

#include "moving_quad.hpp"

namespace aby::eng::sandbox {

	EntryPoint::EntryPoint(int argc, char** argv) :
	    eng::EntryPoint({ .name    = "aby-eng-sandbox",
		                  .version = "1.0",
		                  .argc    = argc,
		                  .argv    = argv }) {
	}

	auto EntryPoint::on_exec() -> void {
		m_Font = Font::create("VeraMono.ttf", 12.f, true);
		Font::set(m_Font);

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
		grid->set_border(2.f, { 0.15f, 0.15f, 1.f, 1.f });

		grid->fill([]() {
			return ui::Image::create(eng::Transform2D({ 0, 0 }, { 18.75f, 18.75f }), "Cobblestone.png");
		});

		auto aspect = ui::AspectRatioBox::create(Transform2D({ 0, 300 }, { 100, 100 }));

		auto button = ui::Button::create(Transform2D({ 0, 300 }, { 100, 100 }),
		                                 Text("hello"),
		                                 ui::ButtonStyle(
		                                     ui::Style(Material2D({ 0.3f, 0.3f, 0.3f, 1.f }),
											           ui::Border(2.f, { 0.15f, 0.15f, 0.15f, 1.f }))));

		aspect->add_child(button);
		canvas->add_child(aspect);
		canvas->add_child(grid);
		App::add_obj(canvas);

		auto window = ui::Window::create(Text("window test"), Rect2D({ 0, 0 }, { 800, 600 }));

		App::add_obj(window);
		App::add_obj(std::make_shared<MovingQuad>());
	}

	auto EntryPoint::on_exit() -> void {
	}

} // namespace aby::eng::sandbox
