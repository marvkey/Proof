#Vertex Shader
#version 450

#include <Common.glslh>
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;

precise invariant gl_Position;

void main()
{
    vec4 worldPos = aTransform * vec4(aPosition, 1.0);
    gl_Position = u_Camera.Projection * u_Camera.View * worldPos;
	
}
#Fragment Shader
#version 450 core

void main()
{
    
}