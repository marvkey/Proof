#Vertex Shader
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    FragPos = vec3(Model * vec4(aPos,1.0));
    Normal = mat3(transpose(inverse(Model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = Projection * View * vec4(FragPos,1.0);
}



#Fragment Shader

#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};

struct Light {
    vec3 Direction;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 ViewPos;
uniform Material material;
uniform Light light;
void main() {
    vec3 Ambient = light.Ambient * texture(material.Diffuse,TexCoords).rgb;

    // diffuse
    vec3 Norm = normalize(Normal);
    vec3 LightDirection = normalize(-light.Direction);
    float Diff = max(dot(Norm,LightDirection),0.0);
    vec3 Diffuse = light.Diffuse * Diff * texture(material.Diffuse,TexCoords).rgb;

    // spectacular
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 ReflectDir = reflect(-LightDirection,Norm);
    float Spec = pow(max(dot(ViewDir,ReflectDir),0.0),material.Shininess);
    vec3 Specular = light.Specular * Spec *texture( material.Specular,TexCoords).rgb;

    vec3 Result = Ambient + Diffuse + Specular;
    FragColor = vec4(Result,1.0);
}