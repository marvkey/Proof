#Vertex Shader
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct OutputBlock
{
    vec2 TexCoord;
};

layout (location = 0) out OutputBlock Output;

void main()
{
    vec4 position = vec4(a_Position.xy, 0.0, 1.0);
    Output.TexCoord = a_TexCoord;
    gl_Position = position;
}


#Fragment Shader
#version 450 core

#include <Common.glslh>

struct OutputBlock
{
    vec2 TexCoord;
};

//ouputs
layout(location = 0) out vec4 out_FragColor;

layout (location = 0) in OutputBlock Input;

//uniforms
layout (binding = 5) uniform sampler2D u_WorldTexture;
layout (binding = 6) uniform sampler2D u_BloomTexture;
layout (binding = 7) uniform sampler2D u_DOFTexture;
layout (binding = 8) uniform sampler2D u_DepthTexture;


float LinearizeDepth(const float screenDepth)
{
    float depthLinearizeMul = u_Camera.DepthUnpackConsts.x;
    float depthLinearizeAdd = u_Camera.DepthUnpackConsts.y;

    return depthLinearizeMul / (depthLinearizeAdd - screenDepth);
}


vec3 UpsampleTent9(sampler2D tex, float lod, vec2 uv, vec2 texelSize, float radius)
{
    vec4 offset = texelSize.xyxy * vec4(1.0f, 1.0f, -1.0f, 0.0f) * radius;

    // Center
    vec3 result = textureLod(tex, uv, lod).rgb * 4.0f;

    result += textureLod(tex, uv - offset.xy, lod).rgb;
    result += textureLod(tex, uv - offset.wy, lod).rgb * 2.0;
    result += textureLod(tex, uv - offset.zy, lod).rgb;

    result += textureLod(tex, uv + offset.zw, lod).rgb * 2.0;
    result += textureLod(tex, uv + offset.xw, lod).rgb * 2.0;

    result += textureLod(tex, uv + offset.zy, lod).rgb;
    result += textureLod(tex, uv + offset.wy, lod).rgb * 2.0;
    result += textureLod(tex, uv + offset.xy, lod).rgb;

    return result * (1.0f / 16.0f);
}


// Based on http://www.oscars.org/science-technology/sci-tech-projects/aces
vec3 ACESTonemap(vec3 color)
{
    mat3 m1 = mat3(
    0.59719, 0.07600, 0.02840,
    0.35458, 0.90834, 0.13383,
    0.04823, 0.01566, 0.83777
    );

    mat3 m2 = mat3(
    1.60475, -0.10208, -0.00327,
    -0.53108,  1.10813, -0.07276,
    -0.07367, -0.00605,  1.07602
    );

    vec3 v = m1 * color;
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;

    return clamp(m2 * (a / b), 0.0, 1.0);
}


vec3 GammaCorrect(vec3 color, float gamma)
{
    return pow(color, vec3(1.0f / gamma));
}


layout(push_constant) uniform Uniforms
{
    float BloomIntensity;
} u_Uniforms;


#define PROOF_RENDERER_FOG_EFFECT 0

void main()
{
    const float gamma = 2.2;

    vec3 color = texture(u_WorldTexture, Input.TexCoord).rgb;

    float sampleScale = 0.5;

    ivec2 texSize = textureSize(u_BloomTexture, 0);
    vec2 fTexSize = vec2(float(texSize.x), float(texSize.y));

    vec3 bloom = UpsampleTent9(
    u_BloomTexture,
    0,
    Input.TexCoord,
    1.0f / fTexSize,
    sampleScale
    ) * u_Uniforms.BloomIntensity;


    vec3 dofTexture = texture(u_DOFTexture, Input.TexCoord).rgb;


    #if PROOF_RENDERER_FOG_EFFECT
    {
    float screenDepth = texture(u_DepthTexture, Input.TexCoord).r;
    float depth = LinearizeDepth(screenDepth);
    depth = abs(depth);

    //float fogStartDistance = 5.5f;
    float fogStartDistance = 10.0f;
    float bloomFogStartDistance = 45.0f;
    float fogFallOffDistance = 45.0f;
    float bloomFogFallOffDistance = 35.0f;

    float fogAmount = smoothstep(
    fogStartDistance,
    fogStartDistance + fogFallOffDistance,
    depth
    );

    float fogAmountBloom = smoothstep(
    bloomFogStartDistance,
    bloomFogStartDistance + bloomFogFallOffDistance,
    depth
    );

    // Skybox
    // Don't completely remove fog from the sky.
    // Instead, limit how much fog can cover it so the horizon still blends nicely.
    bool isSkybox =
    screenDepth <= 0.00001f ||
    screenDepth >= 0.99999f;

    if (isSkybox)
    {
       // fogAmount/=2.0f; 
   //    fogAmountBloom/=2.0f;
    }

    vec3 fogColor = vec3(0.52f, 0.72f, 0.88f);

    vec3 bloomClamped = clamp(
    bloom * (1.0f - fogAmountBloom),
    0.0f,
    1.0f
    );

    //float intensity = (bloomClamped.r + bloomClamped.g + bloomClamped.b) / 3.0f;
    float intensity = (bloomClamped.r + bloomClamped.g + bloomClamped.b) / 3.0f;

    fogColor += bloomClamped * intensity * 0.10f;

    color = mix(color, fogColor, fogAmount);

    fogAmountBloom = clamp(fogAmountBloom, 0.0f, 1.0f);
    bloom *= (1.0f - fogAmountBloom);
    }
    #endif


    //color += dofTexture ;

    color += bloom;

    //color += bloom * dirtTexture;


    const float exposure = 0.8f;

    // Exposure should happen while the image is still HDR.
    color *= exposure;


    color = ACESTonemap(color);

    color = GammaCorrect(color.rgb, gamma);


    out_FragColor = vec4(color, 1.0);
}