#version 330 core

uniform sampler2D uColorTex;
uniform vec2 	uSinglepixel;
uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

in Vertex	{
	vec2 texCoord;
} IN;

out vec4 OutFrag;

const float n_weight[5] = float[](0.12, 0.22, 0.32, 0.22, 0.12);

void main(void) {
	//vec3 result = texture(uColorTex, IN.texCoord).rgb * weight[0];
	//if (horizontal) {
	//	for (int i = 1; i < 5; ++i) {
	//		result += texture(uColorTex, IN.texCoord + vec2(uSinglepixel.x * i, 0.0)).rgb * weight[i];
	//		result += texture(uColorTex, IN.texCoord - vec2(uSinglepixel.x * i, 0.0)).rgb * weight[i];
	//	}
	//}
	//else {
	//	for (int i = 1; i < 5; ++i)
	//	{
	//		result += texture(uColorTex, IN.texCoord + vec2(0.0, uSinglepixel.y * i)).rgb * weight[i];
	//		result += texture(uColorTex, IN.texCoord - vec2(0.0, uSinglepixel.y * i)).rgb * weight[i];
	//	}
	//}
	//OutFrag = vec4(result, 1.0);

	vec2 values[5];

	if (horizontal == false) {
		values = vec2[](vec2(0.0, -uSinglepixel.y * 3),
			 vec2(0.0, -uSinglepixel.y * 2), vec2(0.0, uSinglepixel.y),
			 vec2(0.0, uSinglepixel.y * 2), vec2(0.0, uSinglepixel.y * 3));
	}
	else {
		values = vec2[](vec2(-uSinglepixel.x * 3, 0.0),
			 vec2(-uSinglepixel.x * 2, 0.0), vec2(uSinglepixel.x, 0.0),
			 vec2(uSinglepixel.x * 2, 0.0), vec2(uSinglepixel.x * 3, 0.0));
	}

	for (int i = 0; i < 5; i++) {
		 vec4 tmp = texture (uColorTex, IN.texCoord.xy + values[i]);
		 OutFrag += tmp * n_weight[i];
	}

}