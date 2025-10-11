#include <Common.glslh>
#include <PBR/PostProcessShaderBase/PostProcessCommon.glslh>

#define POST_PROCESS_SHADER_FRAGMENT_BASE

// =============================================================
// Input block from vertex shader
// =============================================================
struct OutputBlock {
    vec2 TexCoord;
    vec2 ScreenUV;
    vec4 ClipPosition;
};
layout(location = 0) in OutputBlock Input;

// =============================================================
// Output color
// =============================================================
layout(location = 0) out vec4 out_FragColor;

layout(set = 1, binding = 0) uniform sampler2D u_InputColor;
layout(set = 1, binding = 1) uniform sampler2D u_InputDepth;

struct PostProcessFragmentInput {
    vec2  UV;
    vec3  Color;
};

void Fragment(inout PostProcessFragmentInput data);
void PreEndFragment();

void main() {
    PostProcessFragmentInput data;

    data.UV    = Input.TexCoord;
    data.Color = texture(u_InputColor, data.UV).rgb;

    Fragment(data); // user-defined effect

    out_FragColor = vec4(data.Color, 1.0);

    PreEndFragment(); // optional
}
