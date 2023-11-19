#Compute Shader
#version 450 core
#include <Common.glslh>

//https://blog.voxagon.se/2018/05/04/bokeh-depth-of-field-in-single-pass.html

layout(binding = 0, rgba32f) writeonly uniform image2D o_Image;
layout (binding = 1) uniform sampler2D u_Texture;
layout (binding = 2) uniform sampler2D u_DepthTexture;

layout(push_constant) uniform Uniforms
{
	vec2 DOFParams; // x = FocusDistance, y = BlurSize
} u_Uniforms;

const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 20.0;
const float RAD_SCALE = 1.0; // Smaller = nicer blur, larger = faster

float ScreenSpaceToViewSpaceDepth(const float screenDepth)
{
	float depthLinearizeMul = -u_Camera.Projection[3][2];
	float depthLinearizeAdd = u_Camera.Projection[2][2];

	//return depthLinearizeMul / (screenDepth +depthLinearizeAdd );
	return depthLinearizeMul / (screenDepth - depthLinearizeAdd );
}

float GetBlurSize(float depth, float focusPoint, float focusScale)
{
	float coc = clamp((1.0 / focusPoint - 1.0 / depth) * focusScale, -1.0, 1.0);
	return abs(coc) * MAX_BLUR_SIZE;
}

vec3 DepthOfField(vec2 texCoord, float focusPoint, float focusScale, vec2 texelSize)
{
	float centerDepth = LinearizeDepth(texture(u_DepthTexture, texCoord).r);
	float centerSize = GetBlurSize(centerDepth, focusPoint, focusScale);
	vec3 color = texture(u_Texture, texCoord).rgb;
	float tot = 1.0;
	float radius = RAD_SCALE;
	for (float ang = 0.0; radius < MAX_BLUR_SIZE; ang += GOLDEN_ANGLE)
	{
		vec2 tc = texCoord + vec2(cos(ang), sin(ang)) * texelSize * radius;
		vec3 sampleColor = texture(u_Texture, tc).rgb;
		float sampleDepth =  LinearizeDepth(texture(u_DepthTexture, tc).r);
		float sampleSize = GetBlurSize(sampleDepth, focusPoint, focusScale);
		if (sampleDepth > centerDepth)
			sampleSize = clamp(sampleSize, 0.0, centerSize * 2.0);
		float m = smoothstep(radius - 0.5, radius + 0.5, sampleSize);
		color += mix(color / tot, sampleColor, m);
		tot += 1.0;
		radius += RAD_SCALE / radius;
	}
	return color /= tot;
}

layout(local_size_x = 32, local_size_y = 32) in;

void main()
{
	ivec2 texSize = imageSize(o_Image);
    ivec2 texCoord = ivec2(gl_GlobalInvocationID.xy);
	float focusPoint = u_Uniforms.DOFParams.x;
	float blurScale = u_Uniforms.DOFParams.y;

	 vec2 normalizedTexCoord = vec2(texCoord) / vec2(texSize);
	vec3 color = DepthOfField(normalizedTexCoord, focusPoint, blurScale, 1.0 / texSize);
    imageStore(o_Image, ivec2(gl_GlobalInvocationID), vec4(color,1.0));
}	