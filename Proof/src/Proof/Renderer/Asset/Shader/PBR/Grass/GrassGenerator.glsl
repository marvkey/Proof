//https://github.com/shineyruan/Vulkan-Grass-Rendering/blob/e9812d64c0535cfb295cc0264c78bbaf72a278a6/src/shaders/compute.comp
#Compute Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable

#define WORKGROUP_SIZE 32
layout(local_size_x = WORKGROUP_SIZE, local_size_y = 1, local_size_z = 1) in;
#include <Common.glslh>

//-------------------------------------------------------------
//https://docs.unity3d.com/Packages/com.unity.shadergraph@7.1/manual/Simple-Noise-Node.html
float unity_noise_randomValue(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

float unity_noise_interpolate(float a, float b, float t)
{
    return (1.0 - t) * a + t * b;
}

float unity_valueNoise(vec2 uv)
{
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    f = f * f * (3.0 - 2.0 * f);

    uv = abs(fract(uv) - 0.5);
    vec2 c0 = i + vec2(0.0, 0.0);
    vec2 c1 = i + vec2(1.0, 0.0);
    vec2 c2 = i + vec2(0.0, 1.0);
    vec2 c3 = i + vec2(1.0, 1.0);

    float r0 = unity_noise_randomValue(c0);
    float r1 = unity_noise_randomValue(c1);
    float r2 = unity_noise_randomValue(c2);
    float r3 = unity_noise_randomValue(c3);

    float bottomOfGrid = unity_noise_interpolate(r0, r1, f.x);
    float topOfGrid = unity_noise_interpolate(r2, r3, f.x);
    return unity_noise_interpolate(bottomOfGrid, topOfGrid, f.y);
}

float Unity_SimpleNoise_float(vec2 UV, float Scale)
{
    float t = 0.0;

    float freq = pow(2.0, 0.0);
    float amp = pow(0.5, 3.0 - 0.0);
    t += unity_valueNoise(vec2(UV.x * Scale / freq, UV.y * Scale / freq)) * amp;

    freq = pow(2.0, 1.0);
    amp = pow(0.5, 3.0 - 1.0);
    t += unity_valueNoise(vec2(UV.x * Scale / freq, UV.y * Scale / freq)) * amp;

    freq = pow(2.0, 2.0);
    amp = pow(0.5, 3.0 - 2.0);
    t += unity_valueNoise(vec2(UV.x * Scale / freq, UV.y * Scale / freq)) * amp;

    return t;
}
//------------------------------------------------------
struct Blade 
{
        // Base of the blade (root position on the surface)
        // v0.xyz = base position (world space)
        // v0.w   = direction angle (rotation around up vector)
        vec4 V0;

        // Control point for curvature (middle of the Bézier curve)
        // v1.xyz = control point position
        // v1.w   = blade height
        vec4 V1;

        // Tip of the blade (moved by physics simulation)
        // v2.xyz = tip position (world space)
        // v2.w   = blade width
        vec4 V2;

        // Blade's up direction and bend stiffness
        // up.xyz = up vector (usually terrain normal)
        // up.w   = stiffness coefficient (resistance to bending)
        vec4 Up;
};

layout(set = 0, binding = 0) buffer InBlades { Blade in_blades[]; } inBlades;

layout(set = 0, binding = 1) buffer CulledBlades { Blade culled_blades[]; } culledBlades;

layout(set = 0, binding = 2) buffer IndirectDrawBlades 
{
  uint vertexCount;    // Write the number of blades remaining here
  uint instanceCount;  // = 1
  uint firstVertex;    // = 0
  uint firstInstance;  // = 0
}numBlades;


bool inBounds(float value, float bounds) {
  return (value >= -bounds) && (value <= bounds);
}

vec3 random3(vec3 p) 
{
  return fract(sin(vec3(dot(p, vec3(127.1, 311.7, 513.76)),
                        dot(p, vec3(269.5, 183.3, 389.22)),
                        dot(p, vec3(378.1, 210.4, 193.9)))) *
               43758.5453);
}


void main()
{
    // Reset the number of blades to 0
    if (gl_GlobalInvocationID.x == 0) 
    {
        numBlades.vertexCount = 0;
    }
memoryBarrierBuffer();
barrier();

    Blade blade = inBlades.in_blades[gl_GlobalInvocationID.x];
    vec3 v0 = blade.V0.xyz; // base (root) position on ground
    vec3 v1 = blade.V1.xyz; // control point (curves the blade)
    vec3 v2 = blade.V2.xyz; // tip of the blade
    vec3 up = blade.Up.xyz; // local up vector (usually terrain normal)

    float orientation = blade.V0.w;   // direction angle around the up vector
    float height = blade.V1.w;   // total blade height
    float width = blade.V2.w;   // blade width
    float stiffness = blade.Up.w;   // recovery force strength (resistance to bending)

    vec3 tangent      = vec3(cos(orientation), 0.0, sin(orientation));
    vec3 front        = cross(tangent, up);

    // ---------------------- GRAVITY FORCES ----------------------
    // Environmental gravity pulling straight down
    vec3 ge = vec3(0.0, -9.81, 0.0);  // environmental gravity for flat plane

    // Add a small gravity force that pushes the blade *forward*
    // Simulates how real grass bends forward under its own weight
    vec3 gf = 0.25 * length(ge) * front;  // front gravity (let blade bend forward)
    vec3 gravity = ge + gf;


    // ---------------------- RECOVERY FORCE ----------------------

    // grass blade's recovery force to natural state
    // Compute the "natural" position of the tip (v2) if no forces act on it
    vec3 iV2 = v0 + height * up;  // natural state

    // Compute how far the tip has been displaced from that ideal position
    // Then pull it back based on the blade's stiffness
    vec3 recovery =(iV2 - v2) * stiffness;  
    //------------------------------------------

    // ---------------------- WIND FORCE ----------------------
    // 1. Compute scrolling UV based on world pos and time
        // Step 1: World UV for sampling wind noise (add time for motion)

            float time = u_FrameData.AppTimeSeconds; // or another time uniform you have

    vec2 windDirection = vec2(-1.0, 0.1);   // wind blowing diagonally
    float windStrength = 25.0;              // controls magnitude of wind
    float windNoiseScale = 5.0;           // scale of procedural noise
    float windNoiseOffset = -0.6;          // additional base strength
    float windSpeed = 5;
    vec2 worldUV = v0.xz + windDirection * time * windSpeed;

    // Step 2: Sample layered noise
    float noiseValue = Unity_SimpleNoise_float(worldUV, windNoiseScale);
    noiseValue = noiseValue * 0.5 + 0.5; // normalize [0, 1]

    // Step 3: Add spatially offset oscillation for natural wave movement
        float phase = dot(v0.xz, windDirection); // offset based on position in wind dir
        float oscillation = sin((time + phase) * windSpeed);
        noiseValue *= oscillation * 0.5 + 0.5;

    // Step 4: Final wind vector
    vec3 wind = vec3(windDirection, 0.0) * (noiseValue + windNoiseOffset) * windStrength;

    // Step 5: Compute blade alignment influence
    float f_d = 1.0 - abs(dot(normalize(wind), normalize(v2 - v0))); // directional influence
    float f_r = dot(v2 - v0, up) / height;                            // how vertical the blade is

// Step 6: Final wind force applied to blade
vec3 wind_force = wind * f_d * f_r;
    /*
    // Sample procedural wind using a random function based on position
    // Output is modulated with time to simulate wind waves
    vec3 wind       = random3(v0) * 3.0 * sin(u_FrameData.AppTimeSeconds);

    // Compute wind direction alignment: if the blade is already bent down,
    // wind should have less effect
    float f_d       = 1 - abs(dot(normalize(wind),normalize(v2 - v0)));  // directional influence
    float f_r       = dot(v2 - v0, up) / height;                   // how vertical the blade is

    // Scale wind force based on blade alignment
    vec3 wind_force = wind * f_d * f_r;
    */
    // ---------------------- APPLY TOTAL FORCE ----------------------
    // total force
    vec3 total_force = recovery + gravity + wind_force;
    //total_force = vec3(0);

    // Apply velocity: advance tip (v2) position using deltaTime
    v2 = v2 + u_FrameData.DeltaTime * total_force;

    // ---------------------- STATE VALIDATION ----------------------

    // --- state validation ---
    // 1. Clamp v2 above ground (can't be pushed through the surface)
    v2 = v2 - up * min(dot(up, v2 - v0), 0.0);

    // 2. Recompute v1 to give a nice curve between v0 and v2
    // This ensures the blade has some curvature based on how much it is bent
    float l_proj = length(v2 - v0 - up * dot(v2 - v0, up));
   // Compute new v1 based on how much the tip is bent
    v1 = v0 + height * up * max(
        1 - l_proj / height, 
        0.05 * max(l_proj / height, 1.0));

    // 3. ensure length of Bezier curve is no larger than the height of blade
    //    use approximation to estimate curve length
    //
    //  Reference: GRAVESEN, J. 1993. Adaptive subdivision and the length of
    //        Bezier curves. Mathematical Institute, Technical University of
    //        Denmark.
    //
   // direct distance between base and tip
    float L0 = length(v2 - v0);
    // sum of distance of adjacent control points
    float L1 = length(v2 - v1) + length(v1 - v0);
    // degree of Bezier curve
    const float n = 2.0; // degree of the curve (quadratic)
    // estimated curve length
    float L = (2 * L0 + (n - 1) * L1) / (n + 1);


    // Rescale the curve to match the blade's defined height
    float r      = height / L;
    vec3 v1_corr = v0 + r * (v1 - v0);
    vec3 v2_corr = v1_corr + r * (v2 - v1);
    // Update v1 and v2 to use the corrected lengths
    v1 = v1_corr;
    v2 = v2_corr;

    // Update blade buffer
    blade.V1.xyz                                = v1;
    blade.V2.xyz                                = v2;
    inBlades.in_blades[gl_GlobalInvocationID.x] = blade;


    //------------------------------------------------------------------------------

     // 1. Orientation test
  vec3 view_dir =
      normalize(vec3(inverse(u_Camera.View) * vec4(0.0, 0.0, 0.0, 1.0)));
  vec3 blade_dir           = vec3(cos(orientation), 0.0, sin(orientation));
  bool culledByOrientation = (abs(dot(view_dir, blade_dir)) < 0.6);

  // 2. View Frustum test
  vec3 midpoint = 0.25 * v0 + 0.5 * v1 + 0.25 * v2;  // midpoint of curve
  const float tolerance = 0.0;
  mat4 view_proj        = u_Camera.Projection * u_Camera.View;

  vec4 v0_ndc = view_proj * vec4(v0, 1.0);
  float h0    = v0_ndc.w + tolerance;
  bool v0_in  = inBounds(v0_ndc.x, h0) && inBounds(v0_ndc.y, h0) &&
               inBounds(v0_ndc.z, h0);

  vec4 v1_ndc = view_proj * vec4(midpoint, 1.0);
  float h1    = v1_ndc.w + tolerance;
  bool v1_in  = inBounds(v1_ndc.x, h1) && inBounds(v1_ndc.y, h1) &&
               inBounds(v1_ndc.z, h1);

  vec4 v2_ndc = view_proj * vec4(v2, 1.0);
  float h2    = v2_ndc.w + tolerance;
  bool v2_in  = inBounds(v2_ndc.x, h2) && inBounds(v2_ndc.y, h2) &&
               inBounds(v2_ndc.z, h2);

  bool culledByViewFrustum = !(v0_in || v1_in || v2_in);

  // 3. Distance Test
  vec3 c =
      vec3(inverse(u_Camera.View) * vec4(0.0, 0.0, 0.0, 1.0));  // camera position
  float d_proj          = length(v0 - c - up * dot(v0 - c, up));
  const float d_max     = 15.0;
  const int num_buckets = 20;
  bool culledByDistance = ((gl_GlobalInvocationID.x % num_buckets) >
                           floor(num_buckets * (1 - d_proj / d_max)));

    // ---------------------- WRITE BACK TO BUFFER ----------------------
   // final output
  //if (!culledByDistance && !culledByOrientation && !culledByViewFrustum) {
    culledBlades.culled_blades[atomicAdd(numBlades.vertexCount, 1)] = blade;
  //}

}
