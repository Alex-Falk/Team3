#version 330 core

uniform sampler2D uColorTex;

uniform vec2 	uSinglepixel;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;

const float exposure = 1.0f;

const float offset = 1.0 / 300;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 OutFrag;

void main(void) {
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
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
		);

	//We need to inverse the gamma function to go back to a non-linear color ramp
	float invGammaCorrection = 1.0 / uGammaCorrection;
	
	vec3 texColor = vec3(0.0f);

	texColor = vec3(texture(uColorTex, IN.texCoord.st));

	if (texColor.r < 1.1 && texColor.g < 1.1 && texColor.b < 1.1) {
		texColor = pow(texColor, vec3(invGammaCorrection));
		texColor = vec3(1.0) - exp(-texColor * exposure);
		OutFrag = vec4(texColor, 1.0f);
	}
	else {
		vec3 sampleTex[9];
		for (int i = 0; i < 9; ++i) {
			sampleTex[i] = vec3(texture(uColorTex, IN.texCoord.st + offsets[i]));
		}

		vec3 col = vec3(0.0f);
		for (int i = 0; i < 9; ++i) {
			col += sampleTex[i] * kernel[i];
		}

		col = pow(col, vec3(invGammaCorrection));

		col = vec3(1.0) - exp(-col * exposure);

		OutFrag = vec4(col, 1.0f);
	}
	//Sample every other pixel and interpolate

	//float samples_taken = 0.0f;
	//for (float x = 0.0f; x < uNumSuperSamples; x += 2.0f)
	//{
	//	for (float y = 0.0f; y < uNumSuperSamples; y += 2.0f)
	//	{
	//		color += texture(uColorTex, IN.texCoord + uSinglepixel * vec2(x, y)).xyz;
	//		samples_taken += 1.0f;
	//	}
	//}
	//color = color / samples_taken;



	//Reinhard HDR algorithm
	//color = color / (color + vec3(1.0));
	
	//Exposure HDR algorithm
	//color = vec3(1.0) - exp(-color * exposure);

	//Inverse Gamma Ramp

	//if (color.r > 0.6 || color.g > 0.6 || color.b > 0.6) {
	//	OutFrag = vec4(color, 1.0f);
	//}
}