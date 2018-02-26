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
uniform sampler2D poolTex;
//position of the different players
uniform uint playerCount;
uniform vec2 players[4];
uniform vec3 colours[4];
//pickups etc, 20 is a random quite high number and can
//be changed to be the exact maximum number of pickups in the future
uniform uint pickupCount;
uniform uint pickupTypes[20];
uniform vec2 pickupPositions[20];

uniform float opacity;
//which player is the one to center the map on
uniform uint self;
//zoom level for map, another one to be variable
uniform float zoom;

//constant stuff
uniform const float POOL_SIZE = 0.05;

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
	//draw pickups
	for(int i = 0; i < int(pickupCount); i++){
		//pickup is a paint pool
		if(pickupTypes[i] == uint(3)){
			//TODO pass through the paint pools
			if(abs(pickupPositions[i].y - newCoords.y) < POOL_SIZE && abs(pickupPositions[i].x - newCoords.x) < POOL_SIZE){
				vec2 poolCoord = vec2(pickupPositions[i].x - newCoords.x + POOL_SIZE, pickupPositions[i].y - newCoords.y + POOL_SIZE)/(POOL_SIZE*2);
				vec4 poolColor = texture(poolTex, poolCoord);
				if(poolColor.a != 0.0f){
					if(poolColor.rgb != vec3(0,0,0)){
						//TODO make this the actual pool's colour
						FragColor.rgb = vec3(0.7,0.05,0.05);
					}
					else{
						FragColor.rgb = poolColor.rgb;
					}
				}
			}
		}
		//else this pickup is a pickup
		else{
			float dist = abs(pickupPositions[i].y - newCoords.y) + abs(pickupPositions[i].x - newCoords.x);
			//jump is blue, weapon is black and speed is yellow
			if(dist < 0.015){
				switch(int(pickupTypes[i])){
					//speed
					case 0:
						FragColor.rgb = vec3(1,1,0);
						break;
					//Jump
					case 1:
						FragColor.rgb = vec3(0,0.7,1);
						break;
					//weapon
					case 2:
						FragColor.rgb = vec3(0,0,0);
						break;
					//everything else is a delightful shade of something is missing
					default:
						FragColor.rgb = vec3(1,0,1);
						break;
				}
				if(dist < 0.01){
					FragColor.rgb = mix(FragColor.rgb, vec3(1,1,1), 0.1);
				}
			}
		}
	}
	//set alpha
	FragColor.a = opacity;
}
