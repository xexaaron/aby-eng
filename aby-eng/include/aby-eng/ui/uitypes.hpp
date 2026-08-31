#pragma once
#include "common.hpp"
#include "misc/types.hpp"

#include <format>
#include <glm/glm.hpp>

namespace aby::eng::ui {

	enum class EStretch {
		fill,  // fills the container by scaling the child non uniformly
		fit,   // fits the object to the container until it reaches a boundary on the x or y axis.
		cover, // scales the object where overflowing pixels are clipped by maintains the original aspect ratio of the object.
	};

	enum class ELayout {
		horizontal, // elements are layed out horizontally
		vertical    // elements are layed out vertically
	};

	enum class EDirection {
		forward,  // left to right, or up to down
		backward, // right to left, or down to up
	};

	enum class EHoverState {
		normal,
		hovered,
		pressed
	};

	struct ABY_API Border {
		explicit Border(float scalar = 0.f, glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f });
		explicit Border(float top, float right, float bottom, float left, glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f });

		auto set_scalar(float scalar = 0.f) -> void;

		float top        = 0.f;
		float right      = 0.f;
		float bottom     = 0.f;
		float left       = 0.f;
		glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f };
	};

	struct ABY_API Style {
		explicit Style(const glm::fvec4& style_color, const Border& border = Border());
		explicit Style(const Material2D& style_material, const Border& border = Border());

		Material2D material;
		Border border;
	};

	struct ABY_API ButtonStyle {
		explicit ButtonStyle(const Style& normal, const Style& hovered, const Style& pressed);
		explicit ButtonStyle(const Style& base);

		Style normal;
		Style hovered;
		Style pressed;
	};

} // namespace aby::eng::ui

namespace std {

	template <>
	struct formatter<aby::eng::ui::EStretch, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::ui::EStretch& e, FmtContext& ctx) const {
			std::string_view str = "<unknown>";
			switch (e) {
				case aby::eng::ui::EStretch::fill:
					str = "fill";
					break;
				case aby::eng::ui::EStretch::fit:
					str = "fit";
					break;
				case aby::eng::ui::EStretch::cover:
					str = "cover";
					break;
			}
			return format_to(ctx.out(), "{}", str);
		}
	};

	template <>
	struct formatter<aby::eng::ui::ELayout, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::ui::ELayout& e, FmtContext& ctx) const {
			std::string_view str = "<unknown>";
			switch (e) {
				case aby::eng::ui::ELayout::horizontal:
					str = "horizontal";
					break;
				case aby::eng::ui::ELayout::vertical:
					str = "vertical";
					break;
			}
			return format_to(ctx.out(), "{}", str);
		}
	};

	template <>
	struct formatter<aby::eng::ui::EDirection, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::ui::EDirection& e, FmtContext& ctx) const {
			std::string_view str = "<unknown>";
			switch (e) {
				case aby::eng::ui::EDirection::forward:
					str = "forward";
					break;
				case aby::eng::ui::EDirection::backward:
					str = "backward";
					break;
			}
			return format_to(ctx.out(), "{}", str);
		}
	};

	template <>
	struct formatter<aby::eng::ui::Border, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::ui::Border& b, FmtContext& ctx) const {
			return format_to(ctx.out(), "(t: {}, r: {}, b: {}, l: {}), (r: {}, g: {}, b: {}, a: {})",
			                 b.top, b.right, b.bottom, b.left, b.color.r, b.color.g, b.color.b, b.color.a);
		}
	};

} // namespace std
