#pragma once
#include "ui/element.hpp"

namespace aby::eng::ui {

	enum class EStretch {
		fill,  // fills the container by scaling the child non uniformly
		fit,   // fits the object to the container until it reaches a boundary on the x or y axis.
		cover, // scales the object where overflowing pixels are clipped by maintains the original aspect ratio of the object.
	};

	struct ABY_API Border {
		Border(float scalar = 0.f, glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f });
		Border(float top, float right, float bottom, float left, glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f });

		auto set_scalar(float scalar = 0.f) -> void;

		float top    = 0.f;
		float right  = 0.f;
		float bottom = 0.f;
		float left   = 0.f;

		glm::fvec4 color = { 1.f, 1.f, 1.f, 1.f };
	};

	class ABY_API Container : public Element {
	public:
		/**
		 * @brief Container constructor
		 * @param transform location, size, & scale
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		Container(Transform2D transform, float padding = 0.f, const Border& border = {}, EStretch stretch = EStretch::fill);

		auto on_tick(const Time& deltatime) -> void override;
		auto on_render() -> void override;

		auto set_stretch(EStretch stretch) -> void;
		auto set_border(const Border& border) -> void;
		auto set_border(float scalar, const glm::fvec4& color) -> void;
		auto set_border(float top, float right, float bottom, float left, const glm::fvec4& color) -> void;
		auto set_padding(float padding) -> void;

		auto available_rect() const -> Rect2D;
		auto stretch() const -> EStretch;
		auto border() const -> const Border&;
		auto padding() const -> float;
	protected:
		float m_Padding;
		Border m_Border;
		EStretch m_Stretch;
	};

} // namespace aby::eng::ui
