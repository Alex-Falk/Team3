/*
           \\\///
	     	  / ~  ~ \
	     	(| (.)(.) |)
.-----.OOOo--C---oOOO.---.
|                        |
|   Made By Philip Beck  |
|      20/02/2018        |
|                        |
'-----.oooO--------------'
      (   )   Oooo.
       \ (    (   )
	     \_)    ) /
	           (_/
*/


#version 150 core

uniform mat4 modelMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
  vec2 texCoord;
} OUT;

 void main(void) {
	vec4 pos = vec4(position.x, -position.y, position.z, 1.0f);
	gl_Position = modelMatrix*pos;
	OUT.texCoord = texCoord;
}
