

#Vertex Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform  ModelBufferObject { mat4 model; };


// Declare vertex shader inputs and outputs
// Inputs must align with CreateGrassPipeline()
layout(location = 0) in vec4 v0;
layout(location = 1) in vec4 v1;
layout(location = 2) in vec4 v2;
layout(location = 3) in vec4 up;

// Outputs are passed to tessellation shaders
layout(location = 0) out vec4 out_v0;
layout(location = 1) out vec4 out_v1;
layout(location = 2) out vec4 out_v2;
layout(location = 3) out vec4 out_up;

out gl_PerVertex { vec4 gl_Position; };

void main() {
  // Write gl_Position and any other shader outputs
  gl_Position = model * vec4(v0.xyz, 1.f);
  out_v0      = vec4((model * vec4(v0.xyz, 1.f)).xyz, v0.w);
  out_v1      = vec4((model * vec4(v1.xyz, 1.f)).xyz, v1.w);
  out_v2      = vec4((model * vec4(v2.xyz, 1.f)).xyz, v2.w);
  out_up      = vec4((model * vec4(up.xyz, 0.f)).xyz, up.w);
}


#TessellationControl Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable

// defines the number of control points in the output patch
layout(vertices = 1) out;

// Declare tessellation control shader inputs and outputs
layout(location = 0) in vec4 list_v0[];
layout(location = 1) in vec4 list_v1[];
layout(location = 2) in vec4 list_v2[];
layout(location = 3) in vec4 list_up[];

layout(location = 0) out vec4 out_v0[];
layout(location = 1) out vec4 out_v1[];
layout(location = 2) out vec4 out_v2[];
layout(location = 3) out vec4 out_up[];

void main() {
  // Don't move the origin location of the patch
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  // Write shader outputs
  // Do not change Bezier control points here
  out_v0[gl_InvocationID] = list_v0[gl_InvocationID];
  out_v1[gl_InvocationID] = list_v1[gl_InvocationID];
  out_v2[gl_InvocationID] = list_v2[gl_InvocationID];
  out_up[gl_InvocationID] = list_up[gl_InvocationID];

  // Set level of tesselation
  gl_TessLevelInner[0] = 8.0;  // horizontal
  gl_TessLevelInner[1] = 8.0;  // vertical
  gl_TessLevelOuter[0] = 8.0;
  gl_TessLevelOuter[1] = 8.0;
  gl_TessLevelOuter[2] = 8.0;
  gl_TessLevelOuter[3] = 8.0;
}


#TessellationEvaluation Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(quads, equal_spacing, ccw) in;
#include <Common.glslh>


// Declare tessellation evaluation shader inputs and outputs
//  The number of control points for each input batch is defined in
//    `layout(vertices = ???) out;`
//  in the tessellation control shader
layout(location = 0) in vec4 v0[];  // v0.w -- orientation
layout(location = 1) in vec4 v1[];  // v1.w -- height
layout(location = 2) in vec4 v2[];  // v2.w -- width
layout(location = 3) in vec4 up[];  // up.w -- stiffness

layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 nor;
layout(location = 2) out vec2 uv;

void main() {
  float u = gl_TessCoord.x;
  float v = gl_TessCoord.y;

  // Use u and v to parameterize along the grass blade and output
  //  positions for each vertex of the grass blade
  // Implements De Casteljau's algorithm as described in paper
  //
  vec3 t1 = vec3(sin(v0[0].w), 0.f, cos(v0[0].w));  // bitangent
  vec3 a  = vec3(v0[0] + v * (v1[0] - v0[0]));
  vec3 b  = vec3(v1[0] + v * (v2[0] - v1[0]));
  vec3 c  = a + v * (b - a);
  vec3 c0 = c - v2[0].w * t1;
  vec3 c1 = c + v2[0].w * t1;
  vec3 t0 = normalize(b - a);          // interpolated tangent
  vec3 n  = normalize(cross(t0, t1));  // interpolated normal

  // vertex for basic shape
  //  Reference: Responsive Real-Time Grass Rendering for General 3D Scenes
  //             Section 6.3, basic shapes
  //
  // Quad:          t = u
  // Triangle:      t = u + 0.5 * v - u * v
  // Quadratic:     t = u - u * v * v
  // Triangle-tip:  t = 0.5 +
  //                  (u - 0.5)*(1 - max(v - threshold, 0)/(1 - threshold))
  //
  // use triangle for best visual effects
  //
  float t = u + 0.5 * v - u * v;
  vec3 p  = mix(c0, c1, t);

  // calculate outputs
  pos         = vec4(p, 1.f);
  nor         = vec4(n, 0.f);
  uv          = vec2(u, v);
  gl_Position = u_Camera.Projection * u_Camera.View * pos;
}

#Fragment Shader
#version 450
#extension GL_ARB_separate_shader_objects : enable
#include <Common.glslh>

// Declare fragment shader inputs
layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 nor;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 outColor;

void main() 
{
  // Compute fragment color
  vec3 dark_green  = vec3(0.2, 0.4, 0.0);
  vec3 light_green = vec3(0.4, 0.8, 0.0);
  vec3 light       = normalize(vec3(1.0, 1.0, 1.0));

  // 1. compute color; mixing from dark to light green
  vec3 color = mix(dark_green, light_green, uv.y);

  // 2. compute lambertian shading
  float lambert_diffuse = dot(vec3(nor), light);
  float lambert_ambient = 0.4;
  float light_intensity = clamp(lambert_diffuse, 0, 1) + lambert_ambient;

  outColor = vec4(light_intensity * color, 1.0);
}