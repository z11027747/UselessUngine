#version 450

layout (push_constant) uniform Push {
    vec4 params;//
} push;

layout (set = 0, binding = 0) uniform sampler2D blitImage;
layout (input_attachment_index = 0, set = 0, binding = 1) uniform subpassInput toonMappingAttachment;
layout (input_attachment_index = 1, set = 0, binding = 2) uniform subpassInput bloomAttachment;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 toonMappingCol = subpassLoad(toonMappingAttachment).rgb;
    vec3 bloomCol = subpassLoad(bloomAttachment).rgb;

    outColor = vec4(toonMappingCol + bloomCol, 1.0);
}