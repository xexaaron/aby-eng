#pragma once
#include "RmlUi/Core/ElementDocument.h"
#include "common.hpp"
#include "event.hpp"

#include <RmlUi/Core.h>
#include <aby-eng/core/object.hpp>
#include <glm/glm.hpp>

namespace aby::eng::editor::rml {

	class RenderInterface : public Rml::RenderInterface {
	public:
		auto CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) -> Rml::CompiledGeometryHandle override;
		auto RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) -> void override;
		auto ReleaseGeometry(Rml::CompiledGeometryHandle geometry) -> void override;
		auto LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) -> Rml::TextureHandle override;
		auto GenerateTexture(Rml::Span<const byte> source, Rml::Vector2i source_dimensions) -> Rml::TextureHandle override;
		auto ReleaseTexture(Rml::TextureHandle texture) -> void override;
		auto EnableScissorRegion(bool enable) -> void override;
		auto SetScissorRegion(Rml::Rectanglei region) -> void override;
	};

	class Renderer : public Object {
	public:
		Renderer(glm::uvec2 window_size);

		static auto create(glm::uvec2 window_size) -> ref<Renderer>;

		/**
		* @brief Load a .rml document into the window
		* @param rel_path Path relative to the resource/ui directory
		* @return true if successful, otherwise false
		*/
		auto load(const std::filesystem::path& rel_path) -> bool;

		auto on_create() -> void override;
		auto on_tick(const Time& dt) -> void override;
		auto on_event(win::Event& event) -> bool override;
		auto on_render() -> void override;
		auto on_destroy() -> void override;
	protected:
		auto on_mouse_scroll(win::MouseScrolledEvent& event) -> bool;
		auto on_mouse_moved(win::MouseMovedEvent& event) -> bool;
		auto on_mouse_pressed(win::MousePressedEvent& event) -> bool;
		auto on_mouse_released(win::MouseReleasedEvent& event) -> bool;
		auto on_mouse_left(win::MouseLeftEvent& event) -> bool;
		auto on_key_pressed(win::KeyPressedEvent& event) -> bool;
		auto on_key_released(win::KeyReleasedEvent& event) -> bool;
		auto on_key_typed(win::KeyTypedEvent& event) -> bool;
		auto on_window_resized(win::WindowResizedEvent& event) -> bool;
	private:
		RenderInterface m_Interface;
		Rml::Context* m_Context;
		glm::uvec2 m_WindowSize;
		std::vector<Rml::ElementDocument*> m_Documents;
	};

} // namespace aby::eng::editor::rml
