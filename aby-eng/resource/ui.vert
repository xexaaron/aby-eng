#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;
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