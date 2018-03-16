// Alex - Falk - Tesselation vertex shader for paintpools/'water'
#version 330 core

uniform mat4 uProjViewMtx;
uniform mat4 uModelMtx;
uniform vec2 uGroundSize;

layout (location = 0) in  vec3 position;
layout (location = 2) in  vec2 texCoord;
layout (location = 3) in  vec3 normal;

out vec3 vPositions;

void main(void)	{
	gl_Position = vec4(position,1.0f);
}