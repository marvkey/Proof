#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos,1.0);
}


#Fragment Shader
#version 450 core
struct DirectionalLight {
    vec3 Direction;
    vec3 Ambient;
    float Intensity;
};

struct PointLight {
    vec3 Position;
    vec3 Ambient;

    float Constant;
    float Linear;
    float Quadratic;

    float Radius;
    float  Intensity;
};

struct SpotLight {
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;

    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    float Radius;
    float Intensity;
};

uniform int v_NrDirectionalLight;
uniform DirectionalLight v_DirectionalLight[150];

uniform int v_NrPointLight;
uniform PointLight v_PointLight[150];

uniform int v_NrSpotLight;
uniform SpotLight v_SpotLight[150];

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMaterialSpec;
layout(std140,binding = 1) uniform CameraData{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;
vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);

void main() {
    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = texture(gNormal,TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec,TexCoords).rgb;
    float Metalliness = texture(gAlbedoSpec,TexCoords).a;
    vec3 viewDir = normalize(Camera.Position - FragPos);

    vec3 result;
    vec3 norm = normalize(Normal);
    for (int i = 0; i < v_NrDirectionalLight; i++)
        result += CalcDirLight(v_DirectionalLight[i],norm,viewDir,Diffuse,Metalliness);

    for (int i = 0; i < v_NrPointLight; i++){
        result += CalcPointLight(v_PointLight[i],norm,FragPos,viewDir,Diffuse,Metalliness);
    }

    for (int i = 0; i < v_NrSpotLight; i++){
        result += CalcSpotLight(v_SpotLight[i],norm,FragPos,viewDir,Diffuse,Metalliness);
    }
    FragColor = vec4(result,1.0);

}

vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    vec3 ambient = light.Ambient * matcolour;                 // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient* matcolour;         // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular = spec * light.Ambient* matcolour;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    return (ambient + diffuse + specular)*light.Intensity;
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 ambient = light.Ambient * matcolour;               // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient * matcolour;        // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular =spec * light.Ambient * matcolour;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular)*light.Intensity;

}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    float theta = dot(lightDir,normalize(-light.Direction));

    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon,0.0,1.0);
    vec3 ambient = light.Ambient * matcolour;
    vec3 diffuse = diff * light.Ambient * matcolour;
    vec3 specular = spec *  light.Ambient* matcolour;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular)*light.Intensity;
}