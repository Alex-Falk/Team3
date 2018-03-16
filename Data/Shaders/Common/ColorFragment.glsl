#version 150 core

out vec4 OutFrag;

uniform vec4 uColor;

void main(void)	{
	OutFrag = uColor;
	OutFrag.a = 1.0f;
}