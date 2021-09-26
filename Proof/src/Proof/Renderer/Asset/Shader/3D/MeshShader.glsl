#Vertex Shader
#version 450 core
layout(location =0) in vec3  aPos;
layout(location =1)in vec2 aTexCoord;
layout(location =2)in vec3 aNormal;
layout(location =3) in mat4 aTransform;
layout(location = 7) in vec3 aMaterialAmbient;
layout(location = 8) in vec3 aMaterialDiffuse;
layout(location = 9) in vec3 aMaterialSpecular;
layout(location = 10) in float aMaterialShinniness;

out vec2 TexCoord;
out vec3 v_MaterialAmbient;
out vec3 v_MaterialDiffuse;
out vec3 v_MaterialSpecular;
out float v_MaterialShininess;

out vec3 FragPos;
out vec3 Normal;

layout(std140,binding=1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 pos;
}Camera;
mat4 temp;
void main() {
	TexCoord = aTexCoord;
	v_MaterialAmbient = aMaterialAmbient;
	v_MaterialDiffuse = aMaterialDiffuse;
	v_MaterialSpecular = aMaterialSpecular;
    v_MaterialShininess = aMaterialShinniness;
	FragPos = vec3(aTransform * vec4(aPos,1.0));
	Normal = mat3(transpose(inverse(aTransform))) * aNormal; // for a normal application this is not good for the shadeer
    // inversing a matrix takes time we are gonna have to send this through this as a uniform or a 
    // as a layout
	gl_Position = Camera.ProjectionMatrix*Camera.ViewMatrix * vec4(FragPos,1.0);
}

#Fragment Shader
#version 450 core
out vec4 FragColor;
uniform sampler2D texture_diffuse;
in vec2 TexCoord;
in vec3 v_MaterialAmbient;
in vec3 v_MaterialDiffuse;
in vec3 v_MaterialSpecular;
in float v_MaterialShininess;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;   
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;  

uniform int v_NrDirectionalLight;
uniform DirLight v_DirectionalLight[150];

uniform int v_NrPointLight;
uniform PointLight v_PointLight[150]; 


uniform int v_NrSpotLight;
uniform SpotLight v_SpotLight[150];


vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
void main() {
	vec3 result;
    vec3 norm = normalize(Normal);
    vec3 viewDir= normalize(viewPos - FragPos);

    for(int i=0; i< v_NrDirectionalLight;i++)
        result+= CalcDirLight(v_DirectionalLight[i],norm,viewDir);

    for(int i=0; i< v_NrPointLight;i++)
        result += CalcPointLight(v_PointLight[i],norm,FragPos,viewDir);

    for(int i=0; i<v_NrSpotLight;i++)
        result += CalcSpotLight(v_SpotLight[i],norm,FragPos,viewDir);

    FragColor = texture(texture_diffuse,TexCoord).rgba*vec4(result,1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal,lightDir),0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),v_MaterialShininess);
    // combine results
    vec3 ambient = light.ambient * v_MaterialAmbient;
    vec3 diffuse = light.diffuse * diff * v_MaterialDiffuse;
    vec3 specular = light.specular * spec * v_MaterialSpecular;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal,lightDir),0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),v_MaterialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * v_MaterialAmbient;
    vec3 diffuse = light.diffuse * diff * v_MaterialDiffuse;
    vec3 specular = light.specular * spec * v_MaterialSpecular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculastes the color when using a spot light.
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal,lightDir),0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),v_MaterialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0,1.0);
    // combine results
    vec3 ambient = light.ambient * v_MaterialAmbient;
    vec3 diffuse = light.diffuse * diff * v_MaterialDiffuse;
    vec3 specular = light.specular * spec * v_MaterialSpecular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}