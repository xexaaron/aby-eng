#pragma once
#include "common.hpp"
#include "misc/font.hpp"
#include "misc/types.hpp"

#include <aby-rhi/aby-rhi.hpp>
#include <glm/glm.hpp>

namespace aby::eng {

	class ABY_API Renderer2D {
	public:
		/**
		 * @brief Upload a quad to be renderered
		 * @param transform The quad transform in pixel coordinates
		 * @param material The rendering material style
		 */
		static auto quad(const Transform2D& transform, const Material2D& material = Material2D()) -> void;
		static auto text(const glm::fvec2& pos, FontPtr font, const Text2D& text) -> void;
	private:
		static auto init() -> bool;
		static auto deinit() -> void;
		static auto begin_frame() -> bool;
		static auto render() -> void;
		static auto end_frame() -> bool;
		friend class App;
	private:
		static inline ref<rhi::RenderPass> m_Pass       = nullptr;
		static inline ref<rhi::VertexBuffer> m_Vertices = nullptr;
		static inline ref<rhi::IndexBuffer> m_Indices   = nullptr;
		static inline rhi::Renderer* m_Renderer         = nullptr;
		static inline rhi::TexturePtr m_DefaultTexture  = nullptr;
	};

} // namespace aby::eng
