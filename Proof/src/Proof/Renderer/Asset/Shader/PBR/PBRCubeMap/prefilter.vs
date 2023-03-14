#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 A_POSITION;

layout(location = 0) out vec3 outPosition;

layout(set = 0, binding = 0) uniform ProjView{
    mat4 projection;
    mat4 view;
} pv;



void main() {
    outPosition = A_POSITION;
    gl_Position = pv.projection * pv.view * vec4(A_POSITION, 1.0f);
}
