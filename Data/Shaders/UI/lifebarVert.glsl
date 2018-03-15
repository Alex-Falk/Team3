#version 330 core

uniform mat4 uModelMtx;
//uniform mat4 textureMatrix;

layout(location = 0) in  vec3 position;
//layout(location = 2) in  vec2 texCoord;

out Vertex{
	vec4 temp_position;
	//vec2 texCoord;
}OUT;

void main(void)	{
	//OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	gl_Position = uModelMtx * vec4(position.x, -position.y, position.z, 1.0);
	OUT.temp_position = gl_Position;
}