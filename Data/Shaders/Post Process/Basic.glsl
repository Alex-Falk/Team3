#version 330 core

uniform sampler2D uColorTex;


uniform vec2 	uSinglepixel;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;

in Vertex	{
	vec2 texCoord;
} IN;

out vec4 OutFrag;

void main(void)	{
	float invGammaCorrection = 1.0 / uGammaCorrection;
	vec3 color = vec3(0.0f);
	color = texture(uColorTex, IN.texCoord).rgb;
	color = pow(color, vec3(invGammaCorrection));
	OutFrag = vec4(color, 1.0f);
}