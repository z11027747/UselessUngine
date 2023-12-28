#version 450

//uniform
layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 projection;

} globalUBO;

layout(push_constant) uniform Push {
    mat4 model;
} push;

//in
layout (location = 0) in vec3 inPosition;

//out
layout (location = 0) out vec3 outUVW;

void main() {
	outUVW = inPosition;
	gl_Position = globalUBO.projection * push.model * vec4(inPosition, 1.0);
}