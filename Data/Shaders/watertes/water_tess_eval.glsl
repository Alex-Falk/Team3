#version 400 core
// Alex - Falk - Tesselation evlauation shader for paintpools/'water'
layout(quads) in;

uniform mat4 uProjViewMtx;
uniform mat4 uModelMtx;
uniform float npatches;
uniform float moveFactor;


uniform sampler2D terrainTex;

// in Vertex {
// 	vec3 tcPos;
// } IN[];

out Vertex{
	smooth vec3 	 worldPos;
	smooth vec2 	 texCoord;
	smooth vec3 	 position;
	smooth vec3 	 normal;
} OUT;

float sinWave(vec2 direction, vec2 position, float amplitude, float frequency, float time, float phase) {
	return amplitude * sin((dot(direction,position)+time)*frequency + phase);
}


void main(void) {

	float u 			= gl_TessCoord.x;
	float v 			= gl_TessCoord.y;

	vec4 a 				= mix(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_TessCoord.x);
	vec4 b				= mix(gl_in[2].gl_Position,gl_in[3].gl_Position,gl_TessCoord.x);


	vec4 pos 			= mix(a,b,gl_TessCoord.y);
	vec2 texCoord 		= (pos.xy+1)/2.0;
	OUT.texCoord 		= texCoord/2;

	vec2 diagonal 		= normalize(vec2(1,1)-vec2(0,0));
	vec2 toCenter		= normalize(vec2(0.5,0.5)-vec2(texCoord.x,texCoord.y));
	 float height 		= sinWave(vec2(1,0), vec2(texCoord.x,texCoord.y),.1, 2,moveFactor, 0.1)
	 						+ sinWave(diagonal, vec2(texCoord.x,texCoord.y),.1, 1,moveFactor, 0.1);

	vec3 position 		= vec3(texCoord, height);

	OUT.position 		= position;
	vec4 wp 			= uModelMtx * vec4(position, 1.0);
	gl_Position 		=  uProjViewMtx * wp;

	OUT.worldPos 		= wp.xyz;
}