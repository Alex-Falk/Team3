#version 330 core

uniform sampler2D uColorTex;

uniform vec2 	uSinglepixel;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;

const float offset = 0.5 / 300.0;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 OutFrag;

void main(void) {
	//We need to inverse the gamma function to go back to a non-linear color ramp
	float invGammaCorrection = 1.0 / uGammaCorrection;


	//Offset array
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	//upper-left
		vec2(0.0f, offset),		//upper-center
		vec2(offset, offset),	//upper-right
		vec2(-offset, 0.0f),	//middle-left
		vec2(0.0f, 0.0f),		//middle-center
		vec2(offset, 0.0f),		//middle-right
		vec2(-offset, -offset), //lower-left
		vec2(0.0f, -offset),	//lower-center
		vec2(offset, -offset)	//lower-right
		);

	//Kernel
	float kernel[9] = float[](
			1,	1, 1,
			1, -8, 1,
			1,	1, 1
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; ++i) {

		sampleTex[i] = vec3(texture(uColorTex, IN.texCoord.st + offsets[i]));
	}

	vec3 col = vec3(0.0f);
	for (int i = 0; i < 9; ++i) {
		col += sampleTex[i] * kernel[i];
	}

	//Inverse Gamma Ramp
	col = pow(col, vec3(invGammaCorrection));

	OutFrag = vec4(col, 1.0f);
}