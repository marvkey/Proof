#Vertex Shader
#version 450
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

#Fragment Shader
#version 450 core
//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.1.2.ibl_irradiance/2.1.2.irradiance_convolution.fs
//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/shaders/env/fIrradiance.frag
layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec4 outColor;
layout(set = 1, binding = 1) uniform samplerCube u_EnvironmentMap;
const float PI = 3.1415926535897932384626433832795f;
const float TWO_PI = PI * 2.0f;
const float HALF_PI = PI * 0.5f;

void main()
{		
	vec3 N = normalize(inPosition);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(up, N));
	up  = normalize(cross(N, right));

	float sampleDelta = 0.025;
	vec3 color = vec3(0.0f);
	uint sampleCount = 0u;
	for (float phi = 0.0f; phi < TWO_PI; phi += sampleDelta) {
		for (float theta = 0.0f; theta < HALF_PI; theta += sampleDelta) {
			 // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 
			color += texture(u_EnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			sampleCount++;
		}
	}
	  color = PI * color * (1.0 / float(sampleCount));
	outColor =  vec4(color, 1.0f);
}