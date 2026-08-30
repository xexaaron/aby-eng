#pragma once
#include "common.hpp"
#include "core/object.hpp"
#include "misc/types.hpp"

#include <span>

namespace aby::eng::ui {

	class ABY_API Element : public Object {
	public:
		using Super = Element;
	public:
		Element();
		Element(const Transform2D& transform);

		/**
		 * @brief Called when the element is created after application initialization.
		 * @note You must call the parent version of this function at the BEGINNING of the overridden method.
		 */
		virtual auto on_create() -> void override;
		/**
		 * @brief Called once per frame to update the element.
		 * @param deltatime The time elapsed since the previous frame.
		 * @note You must call the parent version of this function at the BEGINNING of the overridden method.
		 */
		virtual auto on_tick(const Time& deltatime) -> void override;
		/**
		 * @brief Called once per frame to render the element.
		 * @note You must call the parent version of this function at SOME point during the overriden method.
		 *  	 It entirely depends if you have a background element or an overlay element. 
		 */
		virtual auto on_render() -> void override;
		/**
		 * @brief Called when a window event is dispatched to the element.
		 * @param event The window event being dispatched.
		 * @return True if the event should stop propagating; otherwise false.
		 * @note You must call the parent version of this function at the BEGINNING of the overridden method.
		 */
		virtual auto on_event(win::Event& event) -> bool override;
		/**
		 * @brief Called when the element is destroyed during application shutdown.
		 * @note You must call the parent version of this function at the BEGINNING of the overridden method.
		 */
		virtual auto on_destroy() -> void override;

		auto add_child(ref<Element> element) -> void;
		auto remove_child(ref<Element> element) -> void;

		auto set_transform(const Transform2D& t) -> void;
		auto set_position(const glm::fvec2& t) -> void;
		auto set_size(const glm::fvec2& t) -> void;
		auto set_scale(const glm::fvec2& t) -> void;
		auto set_resolved_layout(Rect2D rect) -> void;

		auto transform() const -> const Transform2D&;
		auto position() const -> const glm::fvec2&;
		auto size() const -> const glm::fvec2&;
		auto scale() const -> const glm::fvec2&;
		auto resolved_layout() const -> Rect2D;
		auto children() const -> std::span<const ref<Element>>;
		auto children() -> std::vector<ref<Element>>&;
	protected:
		Transform2D m_Transform;
		Rect2D m_ResolvedLayout;
		std::vector<ref<Element>> m_Children;
	};

} // namespace aby::eng::ui
