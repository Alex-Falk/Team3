/*
           \\\///
	      / ~  ~ \
		(| (.)(.) |)
.-----.OOOo--C---oOOO.---.
|                        |
|   Made By Philip Beck  |
|      20/02/2018        |
|                        |
'----.oooO---------------'
     (   )            Oooo.
      \ (	    	 (	  )
	   \_)      	  ) /
			         (_/
*/

#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D poolTex;
uniform sampler2D gunTex;
uniform sampler2D jumpTex;
uniform sampler2D speedTex;
uniform sampler2D captureTex;
//position of the different players
uniform uint playerCount;
uniform vec2 players[4];
uniform int colours[4];
//pickups etc, 20 is a random quite high number and can
//be changed to be the exact maximum number of pickups in the future
uniform uint pickupCount;
uniform uint pickupTypes[50];
uniform vec2 pickupPositions[50];
//this wont be used for all pickups
uniform int pickupColours[50];

uniform float opacity;
//which player is the one to center the map on
uniform uint self;
//zoom level for map, another one to be variable
uniform float zoom;
//angle the camera is at
uniform float angle;
//time for making the icons grow and shrink
uniform float time;
//constants
const float BASE_POOL_SIZE = 0.025;
const float BASE_PICKUP_SIZE = 0.025;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColor;

vec2 rotate(vec2 v, float a)
{
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

bool inSquare(vec2 squarePos, vec2 pos, float size){
	//rotate the square back so it doesn't turn with the map
	vec2 diff = abs(rotate(squarePos - pos, -angle));
	if(diff.x < size && diff.y < size){
		return true;
	}
	return false;
}

vec4 drawSquare(vec2 center, vec2 pos, float size, sampler2D tex){
	//get the coords for the square from the center but always draw it at the same angle
	 vec2 texCoords = (rotate(vec2(center.x - pos.x, center.y - pos.y ), -angle) + vec2(size) )/(size*2);
	 vec4 t = texture(tex, texCoords);
	 if(t.a < 1){
		 return vec4(FragColor.rgb, 0);
	 }
	 //return the colour
	 return t;
}

vec3 getColour(int i){
	switch(i){
		case 0:
		//red
			return vec3(1.0f, 0.0f, 0.0f);
		case 1:
		//green
			return vec3(0.0f, 1.0f, 0.0f);
		case 2:
		//blue
			return vec3(0.0f, 0.0f, 1.0f);
		case 3:
		//pink
			return vec3(1.0f, 0.2f, 1.0f);
		default:
			return vec3(0.5f, 0.5f, 0.5f);
	}
}

void main()
{
	//make the pool increase and decrease in size
	float poolSize;
	float pickupSize = BASE_PICKUP_SIZE + (0.005 * sin(time * 2.7));
	//coords relative to the map rather than the fragment
	vec2 newCoords = (rotate(IN.texCoord - 0.5, angle) * zoom) + players[self];
	//check the pixel is on the map
	if(newCoords.x < 0.0f || newCoords.x > 1.0f || newCoords.y < 0.0f || newCoords.y > 1.0f){
			FragColor.rgb = vec3(0,0,0);
	}
	else{
		if(texture(diffuseTex,newCoords).rgb == vec3(0,0,0)){
			//background is grey for now
			FragColor.rgb = vec3(0.4,0.4,0.4);
		}
		else{
	  	FragColor.rgb = texture(diffuseTex,newCoords).rgb;
		}
	}

	//draw pickups
	for(int i = 0; i < int(pickupCount); i++){
		//pickup is a paint pool or capture object (larger and colourable)
		if(pickupTypes[i] > uint(2)){
			//TODO pass through the paint pools
			if (pickupTypes[i] == uint(4))
			{
				poolSize = BASE_POOL_SIZE;
			}
			else
			{
				poolSize = BASE_POOL_SIZE + (0.004 * sin(time * 3));
			}
			if(inSquare(pickupPositions[i], newCoords, poolSize)){
				//use either the pool texture or the capture texture depending on what the "pickup is"
				vec4 poolColor = vec4(1, 1, 1, 1);
				if (pickupTypes[i] == uint(4))
				{
					poolColor.rgb = getColour(pickupColours[i]);
				}
				else
				{
					poolColor = drawSquare(pickupPositions[i], newCoords, poolSize, pickupTypes[i] == uint(3) ? poolTex : captureTex);
				}
				if(!(poolColor.a < 1)){
					if(poolColor.rgb != vec3(0,0,0)){
						//TODO make this the actual pool's colour
						FragColor.rgb = getColour(pickupColours[i]) * 0.7;
					}
					else{
						FragColor.rgb = poolColor.rgb;
					}
				}
			}
		}
		//else this pickup is a pickup
		else{
			//jump is blue, weapon is black and speed is yellow
			if(inSquare(pickupPositions[i], newCoords, pickupSize)){
				switch(int(pickupTypes[i])){
					//speed
					case 0:
						FragColor = drawSquare(pickupPositions[i], newCoords, pickupSize, speedTex);
						break;
					//Jump
					case 1:
						FragColor = drawSquare(pickupPositions[i], newCoords, pickupSize, jumpTex);
						break;
					//weapon
					case 2:
						FragColor = drawSquare(pickupPositions[i], newCoords, pickupSize, gunTex);
						break;
					//everything else is a delightful shade of something is missing
					default:
						FragColor.rgb = vec3(1,0,1);
						break;
				}
			}
		}
	}

	//gives the players a colour
	//the powers are multiplied by zoom so they don't change size depending on how far out the zoom is
	for (int i = 0; i < int(playerCount); i++) {
		float dist = pow(abs(newCoords.x - players[i].x), 2) + pow(abs(newCoords.y - players[i].y), 2);
		vec3 colour = getColour(colours[i]);
		//0.02^2
		if (dist < pow(0.02 * zoom, 2)) {
			FragColor.rgb = colour / 3;
		}
		else if (dist < pow(0.025 * zoom, 2)) {
			FragColor.rgb = colour;
		}
	}

	//set alpha
	FragColor.a = opacity;
}
