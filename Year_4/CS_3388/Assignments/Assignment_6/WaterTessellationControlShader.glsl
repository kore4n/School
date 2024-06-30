#version 440

layout (vertices = 4) out;

// Input vertex data, aggregated into patches
in vec3 Position_worldspace_vs[];
in vec3 Normal_cameraspace_vs[];
in vec3 EyeDirection_worldspace_vs[];
in vec3 LightDirection_worldspace_vs[];
in vec2 uv_vs[];

// Output data : will be passed to TES.
out vec3 position_tcs[];
out vec3 normal_tcs[];
out vec3 eye_tcs[];
out vec3 light_tcs[];
out vec2 uv_tcs[];

// Uniform values that stay constant for the whole mesh.
uniform float outerTess;
uniform float innerTess;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = outerTess;
    gl_TessLevelOuter[1] = outerTess;
    gl_TessLevelOuter[2] = outerTess;
    gl_TessLevelOuter[3] = outerTess;
    gl_TessLevelInner[0] = innerTess;
    gl_TessLevelInner[1] = innerTess;

    position_tcs[gl_InvocationID] = Position_worldspace_vs[gl_InvocationID];
    normal_tcs[gl_InvocationID] = Normal_cameraspace_vs[gl_InvocationID];
    eye_tcs[gl_InvocationID] = EyeDirection_worldspace_vs[gl_InvocationID];
    light_tcs[gl_InvocationID] = LightDirection_worldspace_vs[gl_InvocationID];
    uv_tcs[gl_InvocationID] = uv_vs[gl_InvocationID];
}