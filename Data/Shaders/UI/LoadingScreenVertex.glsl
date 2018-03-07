#version 330 core

uniform mat4 modelMatrix;
uniform mat4 textureMatrix;

layout(location = 0) in  vec3 position;
layout(location = 2) in  vec2 texCoord;

out Vertex{
	//vec3 origin_position;
	vec2 texCoord;
}OUT;

void main(void)	{
	//OUT.texCoord.x = texCoord.x;
	//OUT.texCoord.y = texCoord.y;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	gl_Position = modelMatrix * vec4(position.x, -position.y, position.z, 1.0);;
	//gl_Position = modelMatrix * vec4(position.x, -position.y, position.z, 1.0);
}