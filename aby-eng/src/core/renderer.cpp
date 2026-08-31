#include "core/renderer.hpp"

#include "core/app.hpp"
#include "log.hpp"

#include <glm/ext/matrix_clip_space.hpp>

namespace aby::eng {

	glm::mat4 create_2d_projection(float width, float height) {
		return glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
	}

	auto Renderer2D::init() -> bool {
		auto& ctx  = rhi::Context::get();
		m_Renderer = ctx.renderer();

		// Create and access the texture immediately so that it gets slot 0
		// Then we dont have to check if our vertex id is = to some magic value
		// in order to "not have one"
		m_DefaultTexture            = rhi::Texture::create("default_texture.png", rhi::TextureParams{});
		uint32_t default_texture_id = m_DefaultTexture->id();
		(void)default_texture_id;

		{ // Render pass 2D
			using namespace aby::rhi;
			auto rpb = rhi::RenderPassBuilder::create();
			m_Pass   = rpb->add_shader("ui.vert")
			               .add_shader("ui.frag")
			               .add_push_constant<glm::mat4>("projection")
			               .add_vertex_input<&Vertex2D::pos>(EFormat::rgb_f32)
			               .add_vertex_input<&Vertex2D::uv>(EFormat::rg_f32)
			               .add_vertex_input<&Vertex2D::color>(EFormat::rgba_f32)
			               .add_vertex_input<&Vertex2D::tex>(EFormat::r_u32)
			               .add_color_attachment(Texture::create_render_target(4, EAntiAliasing::msaa4x), true)
			               .set_topology(ETopology::triangle_list)
			               .set_cull_mode(ECullMode::none, EFrontFace::counter_clockwise)
			               .set_polygon_mode(EPolygonMode::fill, 1.f)
			               .set_blend_mask(EChannels::rgba, 0)
			               .set_blend_color(true, Blend{ .op = EBlendOp::add, .src = EBlendFactor::src_alpha, .dst = EBlendFactor::one_minus_src_alpha }, 0)
			               .set_blend_alpha(Blend{ .op = EBlendOp::add, .src = EBlendFactor::one, .dst = EBlendFactor::one_minus_src_alpha }, 0)
			               .set_depth(false, false, ECompareOp::never)
			               .set_stencil(false, ECompareOp::never)
			               .set_antialiasing(EAntiAliasing::msaa4x)
			               .set_depth_format(EFormat::none)
			               .build();

			m_Renderer->add_pass(m_Pass);
		}

		constexpr size_t max_quads = 25'000;
		m_Vertices                 = rhi::VertexBuffer::create<Vertex2D>(max_quads * 4);
		m_Indices                  = rhi::IndexBuffer::create(max_quads * 6);
		return true;
	}

	auto Renderer2D::quad(const Transform2D& transform, const Material2D& material) -> void {
		const auto& pos   = transform.pos;
		const auto& scale = transform.scale;
		const auto& uv    = material.uv;
		const auto& color = material.color;
		const auto& tex   = material.texture;

		const auto sz     = transform.scaled_size();
		const auto offset = static_cast<uint32_t>(m_Vertices->count());

		// uv = { min_u, min_v, max_u, max_v }

		Vertex2D vertices[4] = {
			{			   { pos.x, pos.y }, { uv.x, uv.y }, color, tex }, // top-left
			{        { pos.x + sz.x, pos.y }, { uv.z, uv.y }, color, tex }, // top-right
			{ { pos.x + sz.x, pos.y + sz.y }, { uv.z, uv.w }, color, tex }, // bottom-right
			{        { pos.x, pos.y + sz.y }, { uv.x, uv.w }, color, tex }  // bottom-left
		};

		uint32_t indices[6] = {
			offset + 0,
			offset + 1,
			offset + 2,
			offset + 2,
			offset + 3,
			offset + 0
		};

		m_Indices->push(indices);
		m_Vertices->push(vertices);
	}

	auto Renderer2D::deinit() -> void {
	}

	auto Renderer2D::begin_frame() -> bool {
		m_Indices->clear();
		m_Vertices->clear();

		if (!m_Renderer->on_begin()) {
			return false;
		}

		return true;
	}

	auto Renderer2D::render() -> void {
		auto [w, h]     = App::window()->size();
		auto projection = create_2d_projection(w, h);

		m_Vertices->upload();
		m_Indices->upload();

		m_Pass->push_constant("projection", projection);

		if (m_Vertices->count() > 0) {
			rhi::DrawCmd cmd(m_Vertices, m_Indices, 1);
			m_Pass->submit(cmd);
		}
	}

	auto Renderer2D::end_frame() -> bool {
		return m_Renderer->on_end();
	}

} // namespace aby::eng
