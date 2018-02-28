#version 430 core


in Vertex	{
	smooth vec2 	 texCoord;
} IN;

uniform vec4 uColor;
uniform sampler2D	diffuseTex;

out vec4 FragColor;

layout(binding = 0, offset = 0) uniform atomic_uint scores[4];

void main(void)	{

	vec4 color = texture(diffuseTex,IN.texCoord);

	if (color == vec4(1,0,0,1))
	{
		atomicCounterIncrement(scores[0]);
	}
	else if (color == vec4(0,1,0,1))
	{
		atomicCounterIncrement(scores[1]);
	}
	else if (color == vec4(0,0,1,1))
	{
		atomicCounterIncrement(scores[2]);
	}

	FragColor = color;
}