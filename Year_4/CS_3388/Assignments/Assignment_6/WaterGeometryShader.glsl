#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

// Input vertex data, aggregated into triangles
in vec3 position_tes[];
in vec3 normal_tes[];
in vec3 eye_tes[];
in vec3 light_tes[];
in vec2 uv_tes[];

// Output data per vertex, passed to primitive assembly and rasterization
out vec3 position_gs;
out vec3 normal_gs;
out vec3 eye_gs;
out vec3 light_gs;
out vec2 uv_gs;

// Uniform values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform float time;

vec3 GetNormal(vec3 a, vec3 b, vec3 c)
{
   vec3 x = b.xyz - a.xyz;
   vec3 y = c.xyz - b.xyz;
   return normalize(cross(x, y));
}

vec3 Gerstner(vec3 worldpos, float w, float A, float phi, float Q, vec2 D, int N) {
	float cosThing = cos(dot(w * D, worldpos.xz) + phi * time);
	float sinThing = sin(dot(w * D, worldpos.xz) + phi * time);

	float Qi = Q / w * A * float(N);

	float x = Qi * A * D.x * cosThing;
	float y = A * sinThing;
	float z = Qi * A * D.y * cosThing;

	return vec3(x, y, z);
}

void main() {
	vec3 pos[3];
	for (int i = 0; i < gl_in.length(); ++i) {

		pos[i] = position_tes[i];

		pos[i] += Gerstner(position_tes[i], 4, 0.08, 1.1, 0.75, vec2(0.3, 0.6), 4), 0.0;
		pos[i] += Gerstner(position_tes[i], 2, 0.05, 1.1, 0.75, vec2(0.2, .866), 4), 0.0;
		pos[i] += Gerstner(position_tes[i], 0.6, 0.2, 0.4, 0.1, vec2(0.3, 0.7), 4), 0.0;
		pos[i] += Gerstner(position_tes[i], 0.9, 0.15, 0.4, 0.1, vec2(0.8, 0.1), 4), 0.0;
	}

	vec3 mynorm = GetNormal(pos[0], pos[1], pos[2]);

for (int i = 0; i < gl_in.length(); ++i) {
		vec4 worldSpacePos = MVP * (vec4(pos[i], 1) - vec4(0, 0.3, 0, 0));

		position_gs = worldSpacePos.xyz;
		normal_gs = mynorm;
		eye_gs = eye_tes[i];
		light_gs = light_tes[i];
		uv_gs = uv_tes[i];

		gl_Position = worldSpacePos;

		EmitVertex();
	}

	EndPrimitive();
}
