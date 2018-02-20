#version 330 core

uniform mat4 uModelMtx;

layout (location = 0) in  vec3 position;

out Vertex{
	vec4 temp_position;
}OUT;

void main(void)	{
	
	gl_Position = uModelMtx * vec4(position.x, -position.y, position.z, 1.0);
	OUT.temp_position = gl_Position;
}