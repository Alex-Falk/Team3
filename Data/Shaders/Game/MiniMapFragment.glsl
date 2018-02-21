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

uniform sampler2D diffuseTex;
//position of the different players
uniform uint playerCount;
uniform vec2 players[4];

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColor;

void main()
{
  FragColor = texture(diffuseTex,IN.texCoord);
  for(int i = 0; i < int(playerCount); i++){
    float dist = abs(IN.texCoord.x - players[i].x) + abs(IN.texCoord.y - players[i].y);
    if(dist < 0.02){
      FragColor = vec4(texture(diffuseTex,players[i]).rgb/3, 1);
    }
    else if(dist < 0.03){
      FragColor = texture(diffuseTex,players[i]);
    }
  }
}
