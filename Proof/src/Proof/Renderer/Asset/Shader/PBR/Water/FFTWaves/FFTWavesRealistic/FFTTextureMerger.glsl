//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/wavesTexturesMerger.wgsl
#Compute Shader

#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(push_constant) restrict readonly uniform PushConstants {
    float Lambda;
    float DeltaTime;
};

// === Textures ===
layout(binding = 1, rgba16f) uniform writeonly image2D Displacement;
layout(binding = 2, rgba16f) uniform writeonly image2D Derivatives;

layout(binding = 3) uniform sampler2D TurbulenceRead;
layout(binding = 4, rgba16f) uniform writeonly image2D TurbulenceWrite;

layout(binding = 5) uniform sampler2D Dx_Dz;
layout(binding = 6) uniform sampler2D Dy_Dxz;
layout(binding = 7) uniform sampler2D Dyx_Dyz;
layout(binding = 8) uniform sampler2D Dxx_Dzz;

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    vec2 dx_dz = texelFetch(Dx_Dz, id, 0).xy;
    vec2 dy_dxz = texelFetch(Dy_Dxz, id, 0).xy;
    vec2 dyx_dyz = texelFetch(Dyx_Dyz, id, 0).xy;
    vec2 dxx_dzz = texelFetch(Dxx_Dzz, id, 0).xy;

    // Displacement output
    vec4 disp = vec4(Lambda * dx_dz.x, dy_dxz.x, Lambda * dx_dz.y, 0.0);
    imageStore(Displacement, id, disp);

    // Derivatives output
    vec4 deriv = vec4(dyx_dyz.x, dyx_dyz.y, dxx_dzz.x * Lambda, dxx_dzz.y * Lambda);
    imageStore(Derivatives, id, deriv);

    // Turbulence calculation
    float jacobian = (1.0 + Lambda * dxx_dzz.x) * (1.0 + Lambda * dxx_dzz.y) - Lambda * Lambda * dy_dxz.y * dy_dxz.y;

    float turbulence = texelFetch(TurbulenceRead, id, 0).r + DeltaTime * 0.5 / max(jacobian, 0.5);
    turbulence = min(jacobian, turbulence);

    imageStore(TurbulenceWrite, id, vec4(turbulence, turbulence, turbulence, 1.0));
}
