#include "ui/element.hpp"

namespace aby::eng::ui {

	Element::Element() :
	    m_Transform({ 0.f, 0.f }, { 0.f, 0.f }, 1.f) {
	}

	Element::Element(const Transform2D& transform) :
	    m_Transform(transform) {
	}

	auto Element::on_create() -> void {
		for (auto& child : m_Children) {
			child->on_create();
		}
	}

	auto Element::on_tick(const Time& deltatime) -> void {
		for (auto& child : m_Children) {
			child->on_tick(deltatime);
		}
	}

	auto Element::on_render() -> void {
		for (auto& child : m_Children) {
			child->on_render();
		}
	}

	auto Element::on_event(win::Event& event) -> bool {
		for (auto& child : m_Children) {
			if (child->on_event(event)) {
				return true;
			}
		}
		return false;
	}

	auto Element::on_destroy() -> void {
		for (auto& child : m_Children) {
			child->on_destroy();
		}
	}

	auto Element::add_child(ref<Element> element) -> void {
		m_Children.push_back(element);
	}

	auto Element::remove_child(ref<Element> element) -> void {
		auto it = std::find_if(m_Children.begin(), m_Children.end(), [uuid = element->uuid()](ref<Element> e) {
			return e->uuid() == uuid;
		});

		if (it != m_Children.end()) {
			m_Children.erase(it);
		}
	}

	auto Element::set_transform(const Transform2D& t) -> void {
		m_Transform = t;
	}

	auto Element::set_position(const glm::fvec2& pos) -> void {
		m_Transform.pos = pos;
	}

	auto Element::set_size(const glm::fvec2& size) -> void {
		m_Transform.size = size;
	}

	auto Element::set_scale(const glm::fvec2& scale) -> void {
		m_Transform.scale = scale;
	}

	auto Element::set_resolved_layout(Rect2D rect) -> void {
		m_ResolvedLayout = rect;
	}

	auto Element::transform() const -> const Transform2D& {
		return m_Transform;
	}

	auto Element::position() const -> const glm::fvec2& {
		return m_Transform.pos;
	}

	auto Element::size() const -> const glm::fvec2& {
		return m_Transform.size;
	}

	auto Element::scale() const -> const glm::fvec2& {
		return m_Transform.scale;
	}

	auto Element::resolved_layout() const -> Rect2D {
		return m_ResolvedLayout;
	}

	auto Element::children() const -> std::span<const ref<Element>> {
		return m_Children;
	}

	auto Element::children() -> std::vector<ref<Element>>& {
		return m_Children;
	}

} // namespace aby::eng::ui
