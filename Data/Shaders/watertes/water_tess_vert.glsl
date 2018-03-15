#version 330 core

uniform mat4 uProjViewMtx;
uniform mat4 uModelMtx;
uniform vec2 uGroundSize;

layout (location = 0) in  vec3 position;
layout (location = 2) in  vec2 texCoord;
layout (location = 3) in  vec3 normal;

out vec3 vPositions;

// out Vertex	{
// 	smooth vec3 	 worldPos;
// 	smooth vec2 	 texCoord;
// 	smooth vec3 	 normal;
// } OUT;

void main(void)	{
	//vec4 wp 		= uModelMtx * vec4(position, 1.0);
	//gl_Position		= uProjViewMtx * wp;
	gl_Position = vec4(position,1.0f);
	//OUT.worldPos 	= wp.xyz; //Assumes no w component madness
	//OUT.texCoord	= position.xy;
	
	//This is a much quicker way to calculate the rotated normal value, however it only works
	//  when the model matrix has the same scaling on all axis. If this is not the case, use the other method below.
	//OUT.normal		= mat3(uModelMtx) * normal;
	
	// Use this if your objects have different scaling values along the x,y,z axis
	//OUT.normal		  = transpose(inverse(mat3(uModelMtx))) * normalize(normal);
}