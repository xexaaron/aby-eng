#pragma once
#include <format>

namespace aby::eng::shader {

	class ui {
	public:
		static constexpr std::string_view AUTHOR      = "xexaaron";
		static constexpr std::string_view DESCRIPTION = "Basic 2D ui shader";
		static constexpr std::string_view NAME        = "ui";
		static constexpr std::string_view EXTENSIONS  = "GL_EXT_nonuniform_qualifier";

		static constexpr uint32_t GLSL_VERSION          = 450;
		static constexpr uint32_t VERTEX_INPUT_COUNT    = 4;
		static constexpr uint32_t VERTEX_OUTPUT_COUNT   = 4;
		static constexpr uint32_t FRAGMENT_INPUT_COUNT  = VERTEX_OUTPUT_COUNT;
		static constexpr uint32_t FRAGMENT_OUTPUT_COUNT = 2;

		static constexpr std::string_view FRAGMENT = R"(
#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inUV;
layout(location = 2) in flat uint inTexture;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D textures[];

void main() {
    vec4 tex = texture(textures[inTexture], inUV);
    outColor = inColor * tex;
}
    )";

		static constexpr std::string_view VERTEX = R"(
#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in uint inTexture;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outUV;
layout(location = 2) out flat uint outTexture;

layout(push_constant) uniform PushConstants {
    mat4 projection;
} PC;

void main() {
    gl_Position = PC.projection * vec4(inPos, 0.0, 1.0);

    outColor   = inColor;
    outUV      = inUV;
    outTexture = inTexture;
}
    )";
	};

} // namespace aby::eng::shader

namespace std {

	template <>
	struct formatter<aby::eng::shader::ui, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(aby::eng::shader::ui value, FmtContext& ctx) const {
			return std::format_to(ctx.out(),
			                      "Shader Info\n"
			                      "\tName: 				    {}\n"
			                      "\tDescription: 		    {}\n"
			                      "\tGLSL Version: 		    {}\n"
			                      "Shader Attributes\n"
			                      "\tVertex Input Count:    {}\n"
			                      "\tVertex Output Count:   {}\n"
			                      "\tFragment Input Count:  {}\n"
			                      "\tFragment Output Count: {}\n"
			                      "\tExtensions: 			{}\n"
			                      "Shader Metadata\n"
			                      "\tAuthor: 				{}\n",
			                      value.NAME,
			                      value.DESCRIPTION,
			                      value.GLSL_VERSION,
			                      value.VERTEX_INPUT_COUNT,
			                      value.VERTEX_OUTPUT_COUNT,
			                      value.FRAGMENT_INPUT_COUNT,
			                      value.FRAGMENT_OUTPUT_COUNT,
			                      value.AUTHOR);
		}
	};

} // namespace std
