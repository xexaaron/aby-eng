#pragma once
#include "ui/element.hpp"
#include "ui/uitypes.hpp"

namespace aby::eng::ui {

	class ABY_API Container : public Element {
	public:
		/**
		 * @brief Container constructor
		 * @param transform location, size, & scale
		 * @param layout [horizontal | vertical]
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		Container(Transform2D transform, ELayout layout,
		          EDirection direction = EDirection::forward,
		          float spacing = 0.f, float padding = 0.f,
		          const Border& border = {}, EStretch stretch = EStretch::fill);

		/**
		 * @brief Generic Container creation function
		 * @param transform location, size, & scale
		 * @param layout [horizontal | vertical]
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		static auto create(Transform2D transform, ELayout layout,
		                   EDirection direction = EDirection::forward,
		                   float spacing = 0.f, float padding = 0.f,
		                   const Border& border = {}, EStretch stretch = EStretch::fill) -> ref<Container>;

		auto on_tick(const Time& deltatime) -> void override;
		auto on_render() -> void override;

		auto set_stretch(EStretch stretch) -> void;
		auto set_border(const Border& border) -> void;
		auto set_border(float scalar, const glm::fvec4& color) -> void;
		auto set_border(float top, float right, float bottom, float left, const glm::fvec4& color) -> void;
		auto set_padding(float padding) -> void;
		auto set_spacing(float spacing) -> void;

		auto available_rect() const -> Rect2D;
		auto stretch() const -> EStretch;
		auto border() const -> const Border&;
		auto padding() const -> float;
		auto spacing() const -> float;
	protected:
		auto resolve_child_layout(const Element& child, const Rect2D& rect) const -> Rect2D;
	protected:
		ELayout m_Layout;
		EDirection m_Direction;
		float m_Spacing;
		float m_Padding;
		Border m_Border;
		EStretch m_Stretch;
	};

	class HContainer : public Container {
	public:
		/**
		 * @brief Horizontal Container constructor
		 * @param transform location, size, & scale
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		HContainer(Transform2D transform, EDirection direction = EDirection::forward,
		           float spacing = 0.f, float padding = 0.f,
		           const Border& border = {}, EStretch stretch = EStretch::fill);
		/**
		 * @brief Horizontal Container creation function
		 * @param transform location, size, & scale
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		static auto create(Transform2D transform, EDirection direction = EDirection::forward,
		                   float spacing = 0.f, float padding = 0.f,
		                   const Border& border = {}, EStretch stretch = EStretch::fill) -> ref<HContainer>;
	private:
	};

	class VContainer : public Container {
	public:
		/**
		 * @brief Vertical Container constructor
		 * @param transform location, size, & scale
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		VContainer(Transform2D transform, EDirection direction = EDirection::forward,
		           float spacing = 0.f, float padding = 0.f,
		           const Border& border = {}, EStretch stretch = EStretch::fill);
		/**
		 * @brief Vertical Container creation function
		 * @param transform location, size, & scale
		 * @param direction [forward | backward] 
		 * @param spacing the pixel spacing between each element
		 * @param padding the pixel spacing between the top, left, bottom, and right for children
		 * @param border the border width for each side
		 * @param stretch the constraint mode for how children should be constrained
		 */
		static auto create(Transform2D transform, EDirection direction = EDirection::forward,
		                   float spacing = 0.f, float padding = 0.f,
		                   const Border& border = {}, EStretch stretch = EStretch::fill) -> ref<VContainer>;
	private:
	};

} // namespace aby::eng::ui
