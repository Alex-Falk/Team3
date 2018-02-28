#version 150 core

uniform sampler2D diffuseTex;
uniform vec3 playerColour;

in Vertex	{
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void){
  vec4 tex = texture(diffuseTex, IN.texCoord);
  if(tex.a < 1.0f){
    discard;
  }
  else if(tex.rgb == vec3(1.0f,0.0f,1.0f)){
    gl_FragColor = vec4(playerColour, 1.0f);
  }
  else{
    gl_FragColor = texture(diffuseTex, IN.texCoord);
  }
}
