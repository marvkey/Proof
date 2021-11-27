#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 5) in mat4 aTransform;

layout(location = 9) in vec3 aAlbedoColour;
layout(location = 10) in float aMetallness;
layout(location = 11) in float aRoughness;
layout(location = 12) in float aAO;
layout(location = 13) in int aMaterialEnabled;


struct Material{
    vec3  MaterialColour;
    float Materialmetallic;
    float Materialroughness;
    float MaterialAO;
    int MaterialEnabled;
};
flat out Material v_Material;
out VS_OUT
{
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPosition;
} vs_in;
layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;

void main() {
    v_Material.MaterialColour = aAlbedoColour;
    v_Material.Materialmetallic = aMetallness;
    v_Material.Materialroughness = aRoughness;
    v_Material.MaterialAO = aAO;
    v_Material.MaterialEnabled =aMaterialEnabled;
    vs_in.TexCoords = aTexCoords;
    vs_in.FragPos = vec3(aTransform * vec4(aPos,1.0));
    vs_in.Normal = mat3(aTransform) * aNormal;
    vs_in.CameraPosition = Camera.Position;

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(vs_in.FragPos,1.0);
}
#Fragment Shader
#version 450 core
out vec4 FragColor;
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
struct Material{
    vec3  MaterialColour;
    float Materialmetallic;
    float Materialroughness;
    float MaterialAO;
    int MaterialEnabled;
};
flat in Material v_Material;
in VS_OUT
{
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPosition;
} fs_in;

vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D DiffuseShader;
void main() {
	vec3 result;
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir= normalize(fs_in.CameraPosition - fs_in.FragPos);
    vec3 materialColor;
    float Metalliness ;
    if(v_Material.MaterialEnabled==1){
        materialColor = texture(albedoMap,fs_in.TexCoords).rgb*v_Material.MaterialColour;
        Metalliness = texture(metallicMap,fs_in.TexCoords).r*v_Material.Materialmetallic;
    }
    else{
        materialColor =texture(DiffuseShader,fs_in.TexCoords).rgb;
        Metalliness  = texture(DiffuseShader,fs_in.TexCoords).a;
    }
    for(int i=0; i< v_NrDirectionalLight;i++)
        result+= CalcDirLight(v_DirectionalLight[i],norm,viewDir,materialColor,Metalliness);

    for(int i=0; i< v_NrPointLight;i++)
        result += CalcPointLight(v_PointLight[i],norm,fs_in.FragPos,viewDir,materialColor,Metalliness);

    for(int i=0; i<v_NrSpotLight;i++)
        result += CalcSpotLight(v_SpotLight[i],norm,fs_in.FragPos,viewDir,materialColor,Metalliness);

    //FragColor = texture(texture_diffuse,fs_in.TexCoord).rgba* vec4(result,1.0);
   // FragColor = texture(DiffuseShader,fs_in.TexCoords);
    FragColor = vec4(result,1.0);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess) {
    /*
    vec3 lightDirection = normalize(light.Position - fragPos);
    float distance = length(light.Position - fragPos);
   
    //float attenuation = GetAttenuation(light.Radius,distance);
    float attenuation = 1.0 / (light.Radius + light.Radius * distance + light.Radius * (light.Radius * distance));

    vec3 diffuse = GetDiffuse(light.Ambient,lightDirection,normal)* attenuation* matcolour;
    vec3 specular = GetSpecular(light.Ambient,shininess,viewDir,lightDirection,normal) *
        attenuation* matcolour;

    vec3 ambient = light.Ambient * matcolour;     
    return (ambient + diffuse +specular) * light.Intensity;
    */
    // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    
    
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 ambient = light.Ambient * matcolour*light.Intensity;               // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient * matcolour*light.Intensity;        // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular =spec * light.Ambient * matcolour*light.Intensity;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
    
}
vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    vec3 ambient = light.Ambient * matcolour;                 // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient * matcolour;         // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular = spec * light.Ambient * matcolour;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    return (ambient + diffuse + specular) * light.Intensity;
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
    return(ambient + diffuse + specular)*light.Intensity;
}