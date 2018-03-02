#version 330 core

uniform sampler2D DiffuseTex;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;
uniform vec2 	uSinglepixel;

in Vertex	{
	//vec3 origin_position;
	vec2 texCoord;
} IN;

out vec4 OutFrag;

void main(void)	{
	float invGammaCorrection = 1.0 / uGammaCorrection;
	vec3 color = vec3(0.0f);

	float samples_taken = 0.0f;
	for (float x = 0.0f; x < uNumSuperSamples; x += 2.0f)
	{
		for (float y = 0.0f; y < uNumSuperSamples; y += 2.0f)
		{
			color += texture(DiffuseTex, IN.texCoord + uSinglepixel * vec2(x, y)).xyz;
			samples_taken += 1.0f;
		}
	}
	color = color / samples_taken;

	color = pow(color, vec3(invGammaCorrection));
	//OutFrag = vec4(color, 1.0f);
	OutFrag = vec4(color, 1.0f);
}