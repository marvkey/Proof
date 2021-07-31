#Vertex Shader
#version 330 core
layout(location =0) in vec3  aPos;
layout(location =1)in vec2 aTexCoord;
layout(location =2)in vec3 aNormal;
layout(location =3) in mat4 aTransform;
layout(location = 7) in vec3 aMaterialAmbient;
layout(location = 8) in vec3 aMaterialDiffuse;
layout(location = 9) in vec3 aMaterialSpecular;
layout(location = 10) in float aMaterialShinniness;
uniform mat4 u_View;
uniform mat4 u_Projection;
out vec2 TexCoord;
out vec3 v_MaterialAmbient;
out vec3 v_MaterialDiffuse;
out vec3 v_MaterialSpecular;
out float v_MaterialShininess;

out vec3 FragPos;
out vec3 Normal;
void main() {
	TexCoord = aTexCoord;
	v_MaterialAmbient = aMaterialAmbient;
	v_MaterialDiffuse = aMaterialDiffuse;
	v_MaterialSpecular = aMaterialSpecular;
    v_MaterialShininess = aMaterialShinniness;
	FragPos = vec3(aTransform * vec4(aPos,1.0));
	Normal = mat3(transpose(inverse(aTransform))) * aNormal;
	gl_Position = u_Projection * u_View * vec4(FragPos,1.0);
}

#Fragment Shader
#version 330 core
out vec4 FragColor;
uniform sampler2D texture_diffuse;
in vec2 TexCoord;
in vec3 v_MaterialAmbient;
in vec3 v_MaterialDiffuse;
in vec3 v_MaterialSpecular;
in float v_MaterialShininess;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
in vec3 FragPos;
in vec3 Normal;
uniform Light light;
uniform vec3 viewPos;
void main() {
    // ambient
    vec3 ambient = light.ambient * v_MaterialAmbient;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse * (diff * v_MaterialDiffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),v_MaterialShininess);
    vec3 specular = light.specular * (spec * v_MaterialSpecular);

    vec3 result = ambient + diffuse + specular;

	FragColor = texture(texture_diffuse,TexCoord)*vec4(result,1.0);
}