#version 330 core

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float	uGammaCorrection;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 FragColor;

const float reduceFactor = 1f;

void main(void) {
	float invGammaCorrection = 1.0 / uGammaCorrection;
	vec3 hdrColor = texture(scene, IN.texCoord).rgb;
	vec3 bloomColor = texture(bloomBlur, IN.texCoord).rgb;

	hdrColor = pow(hdrColor, vec3(invGammaCorrection));
	bloomColor = pow(bloomColor, vec3(invGammaCorrection));

	vec4 finalColor = vec4(hdrColor + bloomColor * reduceFactor, 1.0);

	//finalColor = finalColor / (finalColor + vec4(0.5));

	finalColor.rgb *= 0.6;

	FragColor = finalColor;
}