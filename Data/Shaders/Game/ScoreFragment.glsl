#version 440 core
// Alex - Falk
// Shader to count score of pixels 

in Vertex	{
	smooth vec2 	 texCoord;
} IN;

out vec4 FragColor;

uniform sampler2D	uPathTex;

layout(binding = 0, offset = 0) uniform atomic_uint scores[4];

void main(void)	{

	vec4 color = texture(uPathTex,IN.texCoord);

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
	else if (color == vec4(1.0f, 0.0f, 1.0f,1))
	{
		atomicCounterIncrement(scores[3]);
	}

	FragColor = color;
}