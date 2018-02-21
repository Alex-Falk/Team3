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
uniform vec3 colours[4];
uniform float opacity;
//which player is the one to center the map on
uniform uint self;
//zoom level for map, another one to be variable
uniform float zoom;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColor;

void main()
{
	vec2 newCoords = ((IN.texCoord - 0.5) * zoom) + players[self];
	//check the pixel is on the map
	if(newCoords.x < 0.0f || newCoords.x > 1.0f || newCoords.y < 0.0f || newCoords.y > 1.0f){
			FragColor.rgb = vec3(0,0,0);
	}
	else{

		//TESTING
		if(texture(diffuseTex,newCoords).rgb == vec3(0,0,0)){
			FragColor.rgb = vec3(0.4,0.4,0.4);
		}
		else{
	  	FragColor.rgb = texture(diffuseTex,newCoords).rgb;			
		}
	}

	//gives the players a colour
	//the powers are multiplied by zoom so they don't change size depending on how far out the zoom is
  for(int i = 0; i < int(playerCount); i++){
    float dist = pow(abs(newCoords.x - players[i].x),2) + pow(abs(newCoords.y - players[i].y),2);
		//0.02^2
		if(dist < pow(0.02 * zoom,2)){
      FragColor.rgb = colours[i]/3;
    }
    else if(dist < pow(0.025 * zoom, 2)){
      FragColor.rgb = colours[i];
    }
  }

	//set alpha
	FragColor.a = opacity;
}
