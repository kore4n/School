#version 440

layout (quads, equal_spacing) in;

in vec3 position_tcs[];
in vec3 normal_tcs[];
in vec3 eye_tcs[];
in vec3 light_tcs[];
in vec2 uv_tcs[];

out vec3 position_tes;
out vec3 normal_tes;
out vec3 eye_tes;
out vec3 light_tes;

out vec2 uv_tes;

uniform sampler2D disptex;
uniform float time;

void main() {

    vec4 p0= gl_in[0].gl_Position;
    vec4 p1= gl_in[1].gl_Position;
    vec4 p2= gl_in[2].gl_Position;
    vec4 p3= gl_in[3].gl_Position;

    vec4 m1 = mix(p0, p1, gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec4 m2 = mix(p3, p2, gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    gl_Position = mix(m1, m2, gl_TessCoord.y);

    vec3 c1 = mix(normal_tcs[0], normal_tcs[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec3 c2 = mix(normal_tcs[3], normal_tcs[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    normal_tes = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(position_tcs[0], position_tcs[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(position_tcs[3], position_tcs[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    position_tes = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(eye_tcs[0], eye_tcs[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(eye_tcs[3], eye_tcs[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    eye_tes = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(light_tcs[0], light_tcs[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(light_tcs[3], light_tcs[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    light_tes = mix(c1, c2, gl_TessCoord.y);

    vec2 u1 = mix(uv_tcs[0], uv_tcs[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec2 u2 = mix(uv_tcs[3], uv_tcs[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    uv_tes = mix(u1, u2, gl_TessCoord.y);

    float displacement = clamp(texture(disptex, uv_tes).r, -0.002, 0.002);
    position_tes.y += displacement;
}