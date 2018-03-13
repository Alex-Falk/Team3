#version 430 core

layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer PositionBuffer {
	vec3 positions[];
};

layout(std430, binding = 1) buffer VelocityBuffer {
	vec3 velocities[];
};

uniform float dt;

void main()
{
	uint index = gl_GlobalInvocationID.x;

	vec3 v = velocities[index];
	vec3 p = positions[index];

	v += vec3(0,-4.00f, 0) * dt;
	p += v * dt;

	velocities[index] = v;
	positions[index] = p;
}