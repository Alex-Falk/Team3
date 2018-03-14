#version 430 core

layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer PositionBuffer {
	vec4 positions[];
};

layout(std430, binding = 1) buffer VelocityBuffer {
	vec4 velocities[];
};

layout(std430, binding = 2) buffer StartVelocityBuffer {
	vec4 startVelocities[];
};

uniform float dt;
uniform vec3 startPos;
uniform float maxDist;

void main()
{

	float a;
	float fresh = 0;
	uint index = gl_GlobalInvocationID.x;

	vec3 v = velocities[index].xyz;

	if (abs(v.x) <= 0.0001f && abs(v.y) <= 0.0001f && abs(v.z) <= 0.0001f)
	{
		v = startVelocities[index].xyz;
	}

	vec3 p = positions[index].xyz;

	v += vec3(0,-4.0f, 0) * dt;
	p += v * dt;

	vec3 diff = (p-startPos);
	float dist = sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
	a = max(1.0f - (dist / maxDist),0.0f);

	if(p.y <= 0.0f || dist >= maxDist)
	{
		p = startPos;
		v = startVelocities[index].xyz;
		a = 1;
		fresh = 1;
	}

	velocities[index] = vec4(v,fresh);
	positions[index] = vec4(p,a);
}