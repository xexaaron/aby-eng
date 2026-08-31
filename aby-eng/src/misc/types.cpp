#include "misc/types.hpp"

namespace aby::eng {

	Rect2D::Rect2D(glm::fvec2 pos, glm::fvec2 size) :
	    pos(pos),
	    size(size) {
	}

	auto Rect2D::contains(glm::fvec2 point) const -> bool {
		return point.x >= pos.x &&
		       point.x <= pos.x + size.x &&
		       point.y >= pos.y &&
		       point.y <= pos.y + size.y;
	}

	auto Rect2D::contains(float x, float y) const -> bool {
		return contains({ x, y });
	}

	auto Rect2D::min() const -> glm::fvec2 {
		return pos;
	}

	auto Rect2D::max() const -> glm::fvec2 {
		return pos + size;
	}

	auto Rect2D::center() const -> glm::fvec2 {
		return pos + size * 0.5f;
	}

	auto Rect2D::area() const -> float {
		return size.x * size.y;
	}

	auto Rect2D::intersects(const Rect2D& other) const -> bool {
		return pos.x < other.pos.x + other.size.x &&
		       pos.x + size.x > other.pos.x &&
		       pos.y < other.pos.y + other.size.y &&
		       pos.y + size.y > other.pos.y;
	}

	auto Rect2D::intersection(const Rect2D& other) const -> Rect2D {
		const glm::fvec2 min_pos = glm::max(min(), other.min());
		const glm::fvec2 max_pos = glm::min(max(), other.max());

		if (max_pos.x <= min_pos.x || max_pos.y <= min_pos.y)
			return Rect2D();

		return Rect2D(min_pos, max_pos - min_pos);
	}

	auto Rect2D::expanded(float amount) const -> Rect2D {
		return expanded(glm::fvec2{ amount });
	}

	auto Rect2D::expanded(glm::fvec2 amount) const -> Rect2D {
		return Rect2D(pos - amount, size + amount * 2.f);
	}

	auto Rect2D::inset(float amount) const -> Rect2D {
		return inset(glm::fvec2{ amount });
	}

	auto Rect2D::inset(glm::fvec2 amount) const -> Rect2D {
		return Rect2D(pos + amount, size - amount * 2.f);
	}

	auto Rect2D::translated(glm::fvec2 offset) const -> Rect2D {
		return Rect2D(pos + offset, size);
	}

	auto Rect2D::united(const Rect2D& other) const -> Rect2D {
		const glm::fvec2 min_pos = glm::min(min(), other.min());
		const glm::fvec2 max_pos = glm::max(max(), other.max());

		return Rect2D(min_pos, max_pos - min_pos);
	}

} // namespace aby::eng

namespace aby::eng {

	Transform2D::Transform2D(glm::fvec2 pos, glm::fvec2 size, float scale) :
	    pos(pos),
	    size(size),
	    scale(scale) {
	}

	Transform2D::Transform2D(glm::fvec2 pos, glm::fvec2 size, glm::fvec2 scale) :
	    pos(pos),
	    size(size),
	    scale(scale) {
	}

	auto Transform2D::bounds() const -> Rect2D {
		return Rect2D(pos, size * scale);
	}

	auto Transform2D::center() const -> glm::fvec2 {
		return pos + scaled_size() * 0.5f;
	}

	auto Transform2D::scaled_size() const -> glm::fvec2 {
		return size * scale;
	}

	auto Transform2D::translate(glm::fvec2 offset) -> void {
		pos += offset;
	}

	auto Transform2D::translated(glm::fvec2 offset) const -> Transform2D {
		auto result = *this;
		result.translate(offset);
		return result;
	}

	auto Transform2D::scale_by(float amount) -> void {
		scale *= amount;
	}

	auto Transform2D::scale_by(glm::fvec2 amount) -> void {
		scale *= amount;
	}

	auto Transform2D::scaled(float amount) const -> Transform2D {
		auto result = *this;
		result.scale_by(amount);
		return result;
	}

	auto Transform2D::scaled(glm::fvec2 amount) const -> Transform2D {
		auto result = *this;
		result.scale_by(amount);
		return result;
	}

	auto Transform2D::set_center(glm::fvec2 center) -> void {
		pos = center - scaled_size() * 0.5f;
	}

} // namespace aby::eng

namespace aby::eng {

	Material2D::Material2D(glm::fvec4 color, uint32_t texture, glm::fvec4 uv) :
	    color(color),
	    texture(texture),
	    uv(uv) {
	}

	Material2D::Material2D(glm::fvec4 color, rhi::TexturePtr texture, glm::fvec4 uv) :
	    color(color),
	    texture(texture->id()),
	    uv(uv) {
	}

	Material2D::Material2D(rhi::TexturePtr texture, glm::fvec4 uv) :
	    color{ 1.f, 1.f, 1.f, 1.f },
	    texture(texture->id()),
	    uv(uv) {
	}

	Material2D::Material2D(uint32_t texture, glm::fvec4 uv) :
	    color{ 1.f, 1.f, 1.f, 1.f },
	    texture(texture),
	    uv(uv) {
	}

} // namespace aby::eng

