#pragma once

#include "common.hpp"

#include <aby-rhi/texture.hpp>
#include <glm/glm.hpp>

namespace aby::eng {

	struct ABY_API Rect2D {
		explicit Rect2D(glm::fvec2 pos = { 0.f, 0.f }, glm::fvec2 size = { 0.f, 0.f });

		auto contains(glm::fvec2 point) const -> bool;
		auto contains(float x, float y) const -> bool;
		auto min() const -> glm::fvec2;
		auto max() const -> glm::fvec2;
		auto center() const -> glm::fvec2;
		auto area() const -> float;
		auto intersects(const Rect2D& other) const -> bool;
		auto intersection(const Rect2D& other) const -> Rect2D;
		auto expanded(float amount) const -> Rect2D;
		auto expanded(glm::fvec2 amount) const -> Rect2D;
		auto inset(float amount) const -> Rect2D;
		auto inset(glm::fvec2 amount) const -> Rect2D;
		auto translated(glm::fvec2 offset) const -> Rect2D;
		auto united(const Rect2D& other) const -> Rect2D;

		glm::fvec2 pos  = { 0.f, 0.f };
		glm::fvec2 size = { 0.f, 0.f };
	};

	struct ABY_API Transform2D {
		explicit Transform2D(glm::fvec2 pos, glm::fvec2 size, float scale = 1.f);
		explicit Transform2D(glm::fvec2 pos, glm::fvec2 size, glm::fvec2 scale);

		auto bounds() const -> Rect2D;
		auto center() const -> glm::fvec2;
		auto scaled_size() const -> glm::fvec2;
		auto translate(glm::fvec2 offset) -> void;
		auto scale_by(float scale) -> void;
		auto scale_by(glm::fvec2 scale) -> void;
		auto translated(glm::fvec2 offset) const -> Transform2D;
		auto scaled(float scale) const -> Transform2D;
		auto scaled(glm::fvec2 scale) const -> Transform2D;
		auto set_center(glm::fvec2 center) -> void;

		glm::fvec2 pos   = { 0.f, 0.f };
		glm::fvec2 size  = { 0.f, 0.f };
		glm::fvec2 scale = { 1.f, 1.f };
	};

	struct ABY_API Material2D {
		explicit Material2D(glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f }, uint32_t texture = 0, glm::fvec4 uv = { 0.f, 0.f, 1.f, 1.f });
		explicit Material2D(glm::fvec4 color, rhi::TexturePtr texture, glm::fvec4 uv = { 0.f, 0.f, 1.f, 1.f });
		explicit Material2D(rhi::TexturePtr texture, glm::fvec4 uv = { 0.f, 0.f, 1.f, 1.f });
		explicit Material2D(uint32_t texture, glm::fvec4 uv = { 0.f, 0.f, 1.f, 1.f });

		glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f };
		uint32_t texture = 0;                      // texture handle
		glm::fvec4 uv    = { 0.f, 0.f, 0.f, 0.f }; // min, max
	};

	struct ABY_API Vertex2D {
		glm::fvec2 pos   = { 0.f, 0.f };
		glm::fvec2 uv    = { 0.f, 0.f };
		glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f };
		uint32_t tex     = 0; // tex id 0 is valid, it will be a 1x1 white texture
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

	template <>
	struct formatter<aby::eng::Vertex2D, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::Vertex2D& v, FmtContext& ctx) const {
			return format_to(ctx.out(), "(x: {}, y: {}), (u: {}, v: {}), (r: {}, g: {}, b: {}, a: {}), tex: {}",
			                 v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.color.r, v.color.g, v.color.b, v.color.a, v.tex);
		}
	};

} // namespace std
