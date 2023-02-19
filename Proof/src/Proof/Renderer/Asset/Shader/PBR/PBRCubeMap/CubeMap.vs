#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_multiview : enable

layout(location = 0) in vec3 A_POSITION;

layout(location = 0) out vec3 outPosition;

layout(set = 0, binding = 0) uniform ProjView{
    mat4 projection;
    mat4 view[6];
} pv;



void main() {
    outPosition = A_POSITION;
    gl_Position = pv.projection * pv.view[gl_ViewIndex] * vec4(A_POSITION, 1.0f);
}

