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
	OUT.texCoord = (vec4(texCoord, 0.0, 1.0)).xy;
}