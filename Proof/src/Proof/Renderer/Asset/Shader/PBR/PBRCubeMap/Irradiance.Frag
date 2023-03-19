#version 450
//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/shaders/env/fIrradiance.frag
layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 1) uniform samplerCube u_EnvironmentMap;
const float PI = 3.1415926535897932384626433832795f;

float deltaPhi = (2.0f* PI)/180;
float deltaTheta = (0.5f* PI)/64;
void main()
{		
	vec3 N = normalize(inPosition);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(up, N));
	up = cross(right, N);

	const float TWO_PI = PI * 2.0f;
	const float HALF_PI = PI * 0.5f;

	vec3 color = vec3(0.0f);
	uint sampleCount = 0u;
	for (float phi = 0.0f; phi < TWO_PI; phi += deltaPhi) {
		for (float theta = 0.0f; theta < HALF_PI; theta += deltaTheta) {
			vec3 radial = sin(phi) * up + cos(phi) * right;
			vec3 sampleDir = cos(theta) * N + sin(theta) * radial;
			color += texture(u_EnvironmentMap, sampleDir).rgb * cos(theta) * sin(theta);
			sampleCount++;
		}
	}
	outColor =  vec4(PI * color / float(sampleCount), 1.0f);
}