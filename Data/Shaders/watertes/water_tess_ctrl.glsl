#version 400 core
// Alex - Falk - Tesselation control shader for paintpools/'water'
layout(vertices = 4) out;

// in Vertex {
// 	vec3 vPos;
// } IN[];

// out Vertex {
// 	vec3 tcPos;
// } OUT[];

#define ID gl_InvocationID

void main(void) {
	//OUT[ID].tcPos = IN[ID].vPos;
	int num = 128;
	if(ID == 0) {
		gl_TessLevelInner[0] = num;
		gl_TessLevelInner[1] = num;
		gl_TessLevelOuter[0] = num;
		gl_TessLevelOuter[1] = num;
		gl_TessLevelOuter[2] = num;
		gl_TessLevelOuter[3] = num;
	}
	gl_out[ID].gl_Position = gl_in[ID].gl_Position;

}