#pragma once

#include "common.hpp"

#include <aby-rhi/texture.hpp>
#include <glm/glm.hpp>

namespace aby::eng {

	struct ABY_API Transform2D {
		Transform2D(glm::fvec2 pos, glm::fvec2 size, float scale = 1.f);

		glm::fvec2 pos   = { 0.f, 0.f };
		glm::fvec2 size  = { 0.f, 0.f };
		glm::fvec2 scale = { 1.f, 1.f };
	};

	struct ABY_API Material2D {
		Material2D(glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f }, uint32_t texture = 0, glm::fvec4 uv = { 0.f, 0.f, 0.f, 0.f });
		Material2D(glm::fvec4 color, rhi::TexturePtr texture, glm::fvec4 uv = { 0.f, 0.f, 0.f, 0.f });
		Material2D(rhi::TexturePtr texture, glm::fvec4 uv);
		Material2D(uint32_t texture, glm::fvec4 uv);

		glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f };
		uint32_t texture = 0;                      // texture handle
		glm::fvec4 uv    = { 0.f, 0.f, 0.f, 0.f }; // min, max
	};

	struct ABY_API Rect2D {
		Rect2D(glm::fvec2 pos = { 0.f, 0.f }, glm::fvec2 size = { 0.f, 0.f });

		glm::fvec2 pos  = { 0.f, 0.f };
		glm::fvec2 size = { 0.f, 0.f };
	};

} // namespace aby::eng

namespace std {

	template <>
	struct formatter<aby::eng::Transform2D, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::Transform2D& t, FmtContext& ctx) const {
			return format_to(ctx.out(), "(x: {}, y: {}), (w: {}, h: {}), (sx: {}, sy: {})",
			                 t.pos.x, t.pos.y, t.size.x, t.size.y, t.scale.x, t.scale.y);
		}
	};

	template <>
	struct formatter<aby::eng::Material2D, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::Material2D& m, FmtContext& ctx) const {
			return format_to(ctx.out(), "(r: {}, g: {}, b: {}, a: {}), (tex: {}, u: {}, v: {})",
			                 m.color.r, m.color.g, m.color.b, m.color.a, m.texture, m.uv.x, m.uv.y);
		}
	};

	template <>
	struct formatter<aby::eng::Rect2D, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::Rect2D& r, FmtContext& ctx) const {
			return format_to(ctx.out(), "(x: {}, y: {}), (w: {}, h: {})", r.pos.x, r.pos.y, r.size.x, r.size.y);
		}
	};

} // namespace std
