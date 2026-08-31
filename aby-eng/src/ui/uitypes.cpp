#include "ui/uitypes.hpp"

namespace aby::eng::ui {

	Border::Border(float scalar, glm::fvec4 color) :
	    top(scalar),
	    right(scalar),
	    bottom(scalar),
	    left(scalar),
	    color(color) {
	}

	Border::Border(float top, float right, float bottom, float left, glm::fvec4 color) :
	    top(top),
	    right(right),
	    bottom(bottom),
	    left(left),
	    color(color) {
	}

	auto Border::set_scalar(float scalar) -> void {
		this->top    = scalar;
		this->right  = scalar;
		this->bottom = scalar;
		this->left   = scalar;
	}

} // namespace aby::eng::ui

namespace aby::eng::ui {

	auto luminance(const glm::fvec4& color) -> float {
		return 0.2126f * color.r +
		       0.7152f * color.g +
		       0.0722f * color.b;
	}

	auto adjust(const glm::fvec4& color, float amount) -> glm::fvec4 {
		glm::fvec4 result = color;

		if (amount > 0.f) {
			result.r += (1.f - result.r) * amount;
			result.g += (1.f - result.g) * amount;
			result.b += (1.f - result.b) * amount;
		} else {
			const float factor = 1.f + amount;

			result.r *= factor;
			result.g *= factor;
			result.b *= factor;
		}

		return result;
	}

	auto generate_hovered(const glm::fvec4& base) -> glm::fvec4 {
		const bool light = luminance(base) >= 0.5f;
		return adjust(base, light ? -0.08f : 0.08f);
	}

	auto generate_pressed(const glm::fvec4& base) -> glm::fvec4 {
		const bool light = luminance(base) >= 0.5f;
		return adjust(base, light ? -0.16f : 0.16f);
	}

	Style::Style(const Material2D& style_material, const Border& border) :
	    material(style_material),
	    border(border) {
	}

	Style::Style(const glm::fvec4& style_color, const Border& border) :
	    material(style_color),
	    border(border) {
	}

	ButtonStyle::ButtonStyle(const Style& normal, const Style& hovered, const Style& pressed) :
	    normal(normal),
	    hovered(hovered),
	    pressed(pressed) {
	}

	ButtonStyle::ButtonStyle(const Style& base) :
	    normal(base),
	    hovered(base),
	    pressed(base) {
		hovered.material.color = generate_hovered(base.material.color);
		pressed.material.color = generate_pressed(base.material.color);
	}

} // namespace aby::eng::ui
