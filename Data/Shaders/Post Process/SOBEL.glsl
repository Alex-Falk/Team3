#version 330 core

// Alex Falk - sobel shader used for black outlines
uniform sampler2D uColorTex;

uniform vec2 	uSinglepixel;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;

const float offset = 0.3 / 300.0;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 OutFrag;

mat3 sx = mat3(
	1.0, 2.0, 1.0,
	0.0, 0.0, 0.0,
	-1.0, -2.0, -1.0
);
mat3 sy = mat3(
	1.0, 0.0, -1.0,
	2.0, 0.0, -2.0,
	1.0, 0.0, -1.0
);


void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = uSinglepixel.x;
	float h = uSinglepixel.y;

	n[0] = texture(tex, coord + vec2(-w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(w, -h));
	n[3] = texture(tex, coord + vec2(-w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(w, 0.0));
	n[6] = texture(tex, coord + vec2(-w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(w, h));
}

void main(void)
{
	float invGammaCorrection = 1.0 / uGammaCorrection;

	vec4 n[9];
	make_kernel(n, uColorTex, IN.texCoord.st);

	vec3 col = texture(uColorTex, IN.texCoord.st).rgb;

	vec4 sobel_edge_h = (sx[0][0]*n[2]) + (sx[0][1]*n[5]) + (sx[0][2]*n[8]) + (sx[2][0]*n[0]) + (sx[2][1]*n[3]) + (sx[2][2]*n[6]);
	vec4 sobel_edge_v = (sy[0][0]*n[0]) + (sy[1][0]*n[1]) + (sy[2][0]*n[2]) + (sy[0][2]*n[6]) + (sy[1][2]*n[7]) + (sy[2][2]*n[8]);
	
	vec4 sobel = sqrt(pow((sobel_edge_h * sobel_edge_h),vec4(2)) + pow((sobel_edge_v * sobel_edge_v),vec4(2)));

	vec4 finalCol = vec4((col - sobel.rgb), 1.0);

	finalCol = pow(finalCol, vec4(invGammaCorrection));

	if (sobel.r > 0.1 || sobel.g > 0.1 || sobel.b > 0.1)
	{
		OutFrag = vec4(0,0,0,1);
	}
	else 
	{
		OutFrag = finalCol;
	}
	
	
}