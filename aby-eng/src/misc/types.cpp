#include "misc/types.hpp"

namespace aby::eng {

	Transform2D::Transform2D(glm::fvec2 pos, glm::fvec2 size, float scale) :
	    pos(pos),
	    size(size),
	    scale(scale) {
	}

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
