#pragma once

#include "common.hpp"

#include <aby-rhi/texture.hpp>
#include <glm/glm.hpp>

namespace aby::eng {

	struct ABY_API Transform2D {
		Transform2D(glm::fvec2 pos, glm::fvec2 size, float scale = 1.f);

		glm::fvec2 pos  = { 0.f, 0.f };
		glm::fvec2 size = { 0.f, 0.f };
		float scale     = 1.f;
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

} // namespace aby::eng
