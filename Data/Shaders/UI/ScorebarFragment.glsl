#version 330 core

in Vertex{
	vec4 temp_position;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	vec4 player1Color;
	vec4 player2Color;
	vec4 player3Color;
	vec4 player4Color;
	
	player1Color.x = 1;
	player1Color.y = 0;
	player1Color.z = 0;
	player1Color.w = 1;

	if (IN.temp_position.x > 0) {
		gl_FragColor = player1Color;
	}
}