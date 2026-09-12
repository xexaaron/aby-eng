#include "ui/rml.hpp"

#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Vertex.h>
#include <aby-eng/core/renderer.hpp>
#include <aby-eng/log.hpp>
#include <aby-eng/misc/types.hpp>
#include <aby-rhi/buffer.hpp>
#include <aby-rhi/context.hpp>
#include <aby-rhi/draw-cmd.hpp>
#include <aby-rhi/texture.hpp>
#include <aby-win/event.hpp>
#include <memory>

namespace aby::eng::editor::rml::detail {

	auto to_key(win::EKey key) -> Rml::Input::KeyIdentifier;
	auto to_mod(win::EMod mods) -> Rml::Input::KeyModifier;
	auto to_button(win::EMouseButton button) -> int;

} // namespace aby::eng::editor::rml::detail

namespace aby::eng::editor::rml {

	Renderer::Renderer(glm::uvec2 window_size) :
	    m_WindowSize(window_size) {
	}

	auto Renderer::create(glm::uvec2 window_size) -> ref<Renderer> {
		return std::make_shared<Renderer>(window_size);
	}

	auto Renderer::load(const std::filesystem::path& rel_path) -> bool {
		auto rel = fs::path("resource") / "ui" / rel_path;
		auto abs = rhi::Context::get().file_io()->path(rel);
		auto doc = m_Context->LoadDocument(abs);

		if (!doc) {
			log_err("[rml] failed to load document: {}", rel_path);
			return false;
		}

		doc->Show();
		m_Documents.push_back(doc);

		return true;
	}

	auto Renderer::on_create() -> void {
		Rml::SetRenderInterface(&m_Interface);
		if (!Rml::Initialise()) {
			log_err("[rml] failed to initialize");
			return;
		}
		m_Context = Rml::CreateContext("main", Rml::Vector2i(m_WindowSize.x, m_WindowSize.y));
		log_dev("[rml] initialized");

		Rml::LoadFontFace(Font::sys_path() / "Vera.ttf");
	}

	auto Renderer::on_tick(const Time& dt) -> void {
		if (m_Context)
			m_Context->Update();
	}

	auto Renderer::on_event(win::Event& event) -> bool {
		if (m_Context) {
			win::EventDispatcher dsp(event);
			dsp.dispatch(&Renderer::on_mouse_scroll, this);
			dsp.dispatch(&Renderer::on_mouse_moved, this);
			dsp.dispatch(&Renderer::on_mouse_pressed, this);
			dsp.dispatch(&Renderer::on_mouse_released, this);
			dsp.dispatch(&Renderer::on_mouse_left, this);
			dsp.dispatch(&Renderer::on_key_pressed, this);
			dsp.dispatch(&Renderer::on_key_released, this);
			dsp.dispatch(&Renderer::on_key_typed, this);
			dsp.dispatch(&Renderer::on_window_resized, this);
		}
		return false;
	}

	auto Renderer::on_render() -> void {
		if (m_Context)
			m_Context->Render();
	}

	auto Renderer::on_destroy() -> void {
		Rml::Shutdown();
		log_dev("[rml] deinitialized");
	}

	auto Renderer::on_mouse_scroll(win::MouseScrolledEvent& event) -> bool {
		m_Context->ProcessMouseWheel(Rml::Vector2f(event->x, event->y), 0);
		return false;
	}

	auto Renderer::on_mouse_moved(win::MouseMovedEvent& event) -> bool {
		m_Context->ProcessMouseMove(event->x, event->y, 0);
		return false;
	}

	auto Renderer::on_mouse_pressed(win::MousePressedEvent& event) -> bool {
		m_Context->ProcessMouseButtonDown(detail::to_button(event->button), detail::to_mod(event->mods));
		return false;
	}

	auto Renderer::on_mouse_released(win::MouseReleasedEvent& event) -> bool {
		m_Context->ProcessMouseButtonDown(detail::to_button(event->button), detail::to_mod(event->mods));
		return false;
	}

	auto Renderer::on_mouse_left(win::MouseLeftEvent& event) -> bool {
		m_Context->ProcessMouseLeave();
		return false;
	}

	auto Renderer::on_key_pressed(win::KeyPressedEvent& event) -> bool {
		m_Context->ProcessKeyDown(detail::to_key(event->key), detail::to_mod(event->mods));
		return false;
	}

	auto Renderer::on_key_released(win::KeyReleasedEvent& event) -> bool {
		m_Context->ProcessKeyUp(detail::to_key(event->key), detail::to_mod(event->mods));
		return false;
	}

	auto Renderer::on_key_typed(win::KeyTypedEvent& event) -> bool {
		m_Context->ProcessTextInput(event->codepoint);
		return false;
	}

	auto Renderer::on_window_resized(win::WindowResizedEvent& event) -> bool {
		m_Context->SetDimensions(Rml::Vector2i(event->width, event->height));
		return false;
	}

} // namespace aby::eng::editor::rml

namespace aby::eng::editor::rml {

	struct Geometry {
		rhi::DrawCmd cmd;
		bool is_texture_set = false;
	};

	auto RenderInterface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) -> Rml::CompiledGeometryHandle {
		auto vbuff = rhi::VertexBuffer::create<Vertex2D>(vertices.size());
		auto ibuff = rhi::IndexBuffer::create(indices.size());

		for (const auto& v : vertices) {
			auto color = v.colour.ToNonPremultiplied();

			Vertex2D vertex{
				.pos   = { v.position.x, v.position.y },
				.color = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f },
				.uv    = { v.tex_coord.x, v.tex_coord.y },
				.tex   = 0,
			};

			vbuff->push<Vertex2D>(vertex);
		}

		for (const auto index : indices) {
			ibuff->push(static_cast<uint32_t>(index));
		}

		auto* geom = new Geometry{ rhi::DrawCmd(vbuff, ibuff, 1), false };

		return reinterpret_cast<uintptr_t>(geom);
	}

	auto RenderInterface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) -> void {
		auto* geom = reinterpret_cast<Geometry*>(geometry);
		if (!geom->is_texture_set && texture != 0) {
			geom->cmd.vbuff()->for_each<Vertex2D>([texture = texture, translation = translation](Vertex2D* v) {
				auto* tex  = reinterpret_cast<rhi::Texture*>(texture);
				v->tex     = tex->id();
				v->pos.x  += translation.x;
				v->pos.y  += translation.y;
			});
			geom->is_texture_set = true;
		}
		Renderer2D::submit(geom->cmd);
	}

	auto RenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry) -> void {
		auto* cmd = reinterpret_cast<Geometry*>(geometry);
		delete cmd;
	}

	auto RenderInterface::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) -> Rml::TextureHandle {
		auto texture         = rhi::Texture::create(source);
		// immediate texture access, this is blocking and there is no good solution here
		texture_dimensions.x = texture->width();
		texture_dimensions.y = texture->height();
		return reinterpret_cast<Rml::TextureHandle>(texture.get());
	}

	auto RenderInterface::GenerateTexture(Rml::Span<const byte> source, Rml::Vector2i source_dimensions) -> Rml::TextureHandle {
		std::vector<uint8_t> bytes(
		    reinterpret_cast<const uint8_t*>(source.data()),
		    reinterpret_cast<const uint8_t*>(source.data()) + source.size());

		auto texture = rhi::Texture::create(
		    source_dimensions.x,
		    source_dimensions.y,
		    4,
		    std::move(bytes));

		return reinterpret_cast<Rml::TextureHandle>(texture.get());
	}

	auto RenderInterface::ReleaseTexture(Rml::TextureHandle texture) -> void {
		auto tex = reinterpret_cast<rhi::Texture*>(texture);
		tex->destroy();
	}

	auto RenderInterface::EnableScissorRegion(bool enable) -> void {
		Renderer2D::enable_scissor(enable);
	}

	auto RenderInterface::SetScissorRegion(Rml::Rectanglei region) -> void {
		Renderer2D::set_scissor({ region.p0.x, region.p0.y }, { region.p1.x, region.p1.y });
	}

} // namespace aby::eng::editor::rml

namespace aby::eng::editor::rml::detail {

	auto to_key(win::EKey key) -> Rml::Input::KeyIdentifier {
		using enum win::EKey;
		switch (key) {
			case unknown:
				return Rml::Input::KI_UNKNOWN;

			case a:
				return Rml::Input::KI_A;
			case b:
				return Rml::Input::KI_B;
			case c:
				return Rml::Input::KI_C;
			case d:
				return Rml::Input::KI_D;
			case e:
				return Rml::Input::KI_E;
			case f:
				return Rml::Input::KI_F;
			case g:
				return Rml::Input::KI_G;
			case h:
				return Rml::Input::KI_H;
			case i:
				return Rml::Input::KI_I;
			case j:
				return Rml::Input::KI_J;
			case k:
				return Rml::Input::KI_K;
			case l:
				return Rml::Input::KI_L;
			case m:
				return Rml::Input::KI_M;
			case n:
				return Rml::Input::KI_N;
			case o:
				return Rml::Input::KI_O;
			case p:
				return Rml::Input::KI_P;
			case q:
				return Rml::Input::KI_Q;
			case r:
				return Rml::Input::KI_R;
			case s:
				return Rml::Input::KI_S;
			case t:
				return Rml::Input::KI_T;
			case u:
				return Rml::Input::KI_U;
			case v:
				return Rml::Input::KI_V;
			case w:
				return Rml::Input::KI_W;
			case x:
				return Rml::Input::KI_X;
			case y:
				return Rml::Input::KI_Y;
			case z:
				return Rml::Input::KI_Z;

			case num_0:
				return Rml::Input::KI_0;
			case num_1:
				return Rml::Input::KI_1;
			case num_2:
				return Rml::Input::KI_2;
			case num_3:
				return Rml::Input::KI_3;
			case num_4:
				return Rml::Input::KI_4;
			case num_5:
				return Rml::Input::KI_5;
			case num_6:
				return Rml::Input::KI_6;
			case num_7:
				return Rml::Input::KI_7;
			case num_8:
				return Rml::Input::KI_8;
			case num_9:
				return Rml::Input::KI_9;

			case f1:
				return Rml::Input::KI_F1;
			case f2:
				return Rml::Input::KI_F2;
			case f3:
				return Rml::Input::KI_F3;
			case f4:
				return Rml::Input::KI_F4;
			case f5:
				return Rml::Input::KI_F5;
			case f6:
				return Rml::Input::KI_F6;
			case f7:
				return Rml::Input::KI_F7;
			case f8:
				return Rml::Input::KI_F8;
			case f9:
				return Rml::Input::KI_F9;
			case f10:
				return Rml::Input::KI_F10;
			case f11:
				return Rml::Input::KI_F11;
			case f12:
				return Rml::Input::KI_F12;

			case left_shift:
				return Rml::Input::KI_LSHIFT;
			case right_shift:
				return Rml::Input::KI_RSHIFT;
			case left_ctrl:
				return Rml::Input::KI_LCONTROL;
			case right_ctrl:
				return Rml::Input::KI_UNKNOWN;
			case left_alt:
				return Rml::Input::KI_UNKNOWN; // handled via key mods
			case right_alt:
				return Rml::Input::KI_UNKNOWN; // handled via key mods
			case left_super:
				return Rml::Input::KI_LMETA;
			case right_super:
				return Rml::Input::KI_RMETA;

			case up:
				return Rml::Input::KI_UP;
			case down:
				return Rml::Input::KI_DOWN;
			case left:
				return Rml::Input::KI_LEFT;
			case right:
				return Rml::Input::KI_RIGHT;

			case home:
				return Rml::Input::KI_HOME;
			case end:
				return Rml::Input::KI_END;
			case page_up:
				return Rml::Input::KI_PRIOR;
			case page_down:
				return Rml::Input::KI_NEXT;
			case insert:
				return Rml::Input::KI_INSERT;
			case del:
				return Rml::Input::KI_DELETE;

			case backspace:
				return Rml::Input::KI_BACK;
			case enter:
				return Rml::Input::KI_RETURN;
			case tab:
				return Rml::Input::KI_TAB;
			case escape:
				return Rml::Input::KI_ESCAPE;
			case space:
				return Rml::Input::KI_SPACE;

			case apostrophe:
				return Rml::Input::KI_OEM_7;
			case comma:
				return Rml::Input::KI_OEM_COMMA;
			case minus:
				return Rml::Input::KI_OEM_MINUS;
			case period:
				return Rml::Input::KI_OEM_PERIOD;
			case slash:
				return Rml::Input::KI_OEM_2;
			case semicolon:
				return Rml::Input::KI_OEM_1;
			case equal:
				return Rml::Input::KI_OEM_PLUS;
			case left_bracket:
				return Rml::Input::KI_OEM_4;
			case backslash:
				return Rml::Input::KI_OEM_5;
			case right_bracket:
				return Rml::Input::KI_OEM_6;
			case grave_accent:
				return Rml::Input::KI_OEM_3;

			case caps_lock:
				return Rml::Input::KI_CAPITAL;
			case num_lock:
				return Rml::Input::KI_NUMLOCK;
			case scroll_lock:
				return Rml::Input::KI_SCROLL;

			case kp_0:
				return Rml::Input::KI_NUMPAD0;
			case kp_1:
				return Rml::Input::KI_NUMPAD1;
			case kp_2:
				return Rml::Input::KI_NUMPAD2;
			case kp_3:
				return Rml::Input::KI_NUMPAD3;
			case kp_4:
				return Rml::Input::KI_NUMPAD4;
			case kp_5:
				return Rml::Input::KI_NUMPAD5;
			case kp_6:
				return Rml::Input::KI_NUMPAD6;
			case kp_7:
				return Rml::Input::KI_NUMPAD7;
			case kp_8:
				return Rml::Input::KI_NUMPAD8;
			case kp_9:
				return Rml::Input::KI_NUMPAD9;

			case kp_decimal:
				return Rml::Input::KI_DECIMAL;
			case kp_divide:
				return Rml::Input::KI_DIVIDE;
			case kp_multiply:
				return Rml::Input::KI_MULTIPLY;
			case kp_subtract:
				return Rml::Input::KI_SUBTRACT;
			case kp_add:
				return Rml::Input::KI_ADD;
			case kp_enter:
				return Rml::Input::KI_NUMPADENTER;
			case kp_equal:
				return Rml::Input::KI_OEM_NEC_EQUAL;
			case print_screen:
				return Rml::Input::KI_SNAPSHOT;
			case pause:
				return Rml::Input::KI_PAUSE;
			case menu:
				return Rml::Input::KI_APPS;

			case count:
				return Rml::Input::KI_UNKNOWN;
		}

		return Rml::Input::KI_UNKNOWN;
	}

	auto to_mod(win::EMod mods) -> Rml::Input::KeyModifier {
		if (mods == win::EMod::none) return Rml::Input::KeyModifier{};

		int modifiers = 0;
		if ((mods & win::EMod::shift) != win::EMod::none) {
			modifiers |= Rml::Input::KM_SHIFT;
		}
		if ((mods & win::EMod::ctrl) != win::EMod::none) {
			modifiers |= Rml::Input::KM_CTRL;
		}
		if ((mods & win::EMod::alt) != win::EMod::none) {
			modifiers |= Rml::Input::KM_ALT;
		}
		if ((mods & win::EMod::super) != win::EMod::none) {
			modifiers |= Rml::Input::KM_META;
		}
		if ((mods & win::EMod::caps_lock) != win::EMod::none) {
			modifiers |= Rml::Input::KM_CAPSLOCK;
		}
		if ((mods & win::EMod::num_lock) != win::EMod::none) {
			modifiers |= Rml::Input::KM_NUMLOCK;
		}

		return static_cast<Rml::Input::KeyModifier>(modifiers);
	}

	auto to_button(win::EMouseButton button) -> int {
		auto result  = static_cast<int>(button);
		result      -= 1; // none == 0, so subtract by one to get the button index
		return result;
	}

} // namespace aby::eng::editor::rml::detail
