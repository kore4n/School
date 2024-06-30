#version 440

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

// Output data : will be passed to TCS.
out vec3 Position_worldspace_vs;
out vec3 Normal_cameraspace_vs;
out vec3 EyeDirection_worldspace_vs;
out vec3 LightDirection_worldspace_vs;
out vec2 uv_vs;

// Uniform values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;
uniform vec3 CameraPosition_worldspace;
uniform float time;

void main(){
    // // Position of the vertex, in worldspace : M * position
    Position_worldspace_vs = (M * vec4(vertexPosition_modelspace,1)).xyz;

    gl_Position = M * vec4(vertexPosition_modelspace,1);

    EyeDirection_worldspace_vs = CameraPosition_worldspace - Position_worldspace_vs;

    LightDirection_worldspace_vs = LightPosition_worldspace + EyeDirection_worldspace_vs;

    Normal_cameraspace_vs = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

    uv_vs = (Position_worldspace_vs.xz + 0.25 + (time * 0.08)) / 2;
}
