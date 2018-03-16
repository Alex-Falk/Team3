// Alex - Falk - Tesselation geometry for paintpools/'water'
#version 150 core

uniform mat4 uProjViewMtx;
uniform mat4 uModelMtx;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex	{
	smooth vec3 	 worldPos;
	smooth vec2 	 texCoord;
	smooth vec3 	 position;
	smooth vec3 	 normal;
} IN[];

out Vertex	{
	smooth vec3 	 worldPos;
	smooth vec2 	 texCoord;
	smooth vec3 	 position;
	smooth vec3 	 normal;
	smooth vec3 	 tangent;
	smooth vec3 	 binormal;
} OUT;

void main() {

	vec3 v1 = IN[0].position;
	vec3 v2 = IN[1].position;
	vec3 v3 = IN[2].position;

	vec3 wp1 = IN[0].worldPos;
	vec3 wp2 = IN[1].worldPos;
	vec3 wp3 = IN[2].worldPos;

	vec3 A = wp3-wp1;
	vec3 B = wp2-wp1;
	vec3 C = wp3-wp2;

	mat3 normalMatrix	= transpose (inverse(mat3(uModelMtx)));
	vec3 normal 		= abs(normalize(cross(A,B)));
	
	vec3 tangent 		= vec3(0,0,0);


	if (wp3.x == wp1.x) 
	{
		if (wp3.z >= wp1.z) 
		{
			tangent = normalize(A);
		} 
		else 
		{
			tangent = normalize(-A);
		}	
	}
	else if (wp2.x == wp1.x) 
	{
		if (wp2.z >= wp1.z) 
		{
			tangent = normalize(B);
		} 
		else 
		{
			tangent = normalize(-B);
		}	
	} 
	else if (wp3.x == wp2.x) 
	{
		if (wp3.z >= wp2.z) 
		{
			tangent = normalize(C);
		} 
		else 
		{
			tangent = normalize(-C);
		}
	}


	OUT.normal 			= normal;
	OUT.tangent 		= tangent;
	OUT.binormal 		= normalize(cross(normal,tangent));


	OUT.texCoord 		= IN[0].texCoord;
	gl_Position 		= gl_in[0].gl_Position;
	OUT.position 		= v1;
	OUT.worldPos 		= wp1;
	EmitVertex();

	OUT.texCoord 		= IN[1].texCoord;
	gl_Position			= gl_in[1].gl_Position;
	OUT.position 		= v2;
	OUT.worldPos 		= wp2 ;
	EmitVertex();

	OUT.texCoord 		= IN[2].texCoord;
	gl_Position 		= gl_in[2].gl_Position;
	OUT.position 		= v3;
	OUT.worldPos 		= wp3;
	EmitVertex();


	EndPrimitive();
}