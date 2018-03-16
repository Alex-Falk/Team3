//////////////////////////////////////////////////////////////////////////////////////
//
//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                           O\  =  /O
//                        ____/`---'\____
//                      .'  \\|     |//  `.
//                     /  \\|||  :  |||//  \
//                    /  _||||| -:- |||||-  \
//                    |   | \\\  -  /// |   |
//                    | \_|  ''\---/''  |   |
//                    \  .-\__  `-`  ___/-. /
//                  ___`. .'  /--.--\  `. . __
//               ."" '<  `.___\_<|>_/___.'  >'"".
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//              \  \ `-.   \_ __\ /__ _/   .-` /  /
//         ======`-.____`-.___\_____/___.-`____.-'======
//                            `=---='
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//							Jianfei
//////////////////////////////////////////////////////////////////////////////////////

#version 330 core

uniform float player1;
uniform float player2;
uniform float player3;
uniform float player4;
uniform sampler2D DiffuseTex;
uniform sampler2D dudvTex;
uniform float	uGammaCorrection;
uniform float	uNumSuperSamples;
uniform vec2 	uSinglepixel;
uniform float moveFactor;
uniform float isLifebar;
uniform float health;

const float waveStrength = 0.015;

in Vertex{
	vec4 temp_position;
	vec2 texCoord;
} IN;

out vec4 FragColor;

void main(void)	{
	float invGammaCorrection = 1.0 / uGammaCorrection;

	vec4 player1Color = vec4(1, 0, 0, 0.6);
	vec4 player2Color = vec4(0, 1, 0, 0.6);
	vec4 player3Color = vec4(0, 0, 1, 0.6);
	vec4 player4Color = vec4(0.5, 0.5, 0.5, 0.6);

	//calculate texture coords based on dudv map
	vec2 normalTexCoords = vec2(IN.texCoord.x + moveFactor, IN.texCoord.y);
	vec2 distortion1 = (texture(dudvTex, vec2(IN.texCoord.x, IN.texCoord.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 finalTexCoords = normalTexCoords + distortion1;
	vec3 color = texture(DiffuseTex, finalTexCoords).xyz;

	color = pow(color, vec3(invGammaCorrection));

	float a1 = player1;
	float a2 = player2;
	float a3 = player3;
	float a4 = player4;

	float total = a1 + a2 + a3 + a4;
	a1 = a1 / total;
	a2 = a2 / total;
	a3 = a3 / total;
	a4 = a4 / total;
	a2 = a1 + a2;
	a3 = a3 + a2;
	a4 = a4 + a3;
	a1 = a1 * 0.8 - 0.4;
	a2 = a2 * 0.8 - 0.4;
	a3 = a3 * 0.8 - 0.4;
	a4 = a4 * 0.8 - 0.4;
	vec4 finalColor = vec4(color, 1.0);

	if (isLifebar == 0) {
		if (IN.temp_position.x < a1) {
			finalColor.g -= 0.7;
			finalColor.b -= 0.7;
			finalColor.r += 0.7;
			FragColor = finalColor;
		}
		else if (IN.temp_position.x >= a1 && IN.temp_position.x < a2) {
			finalColor.r -= 0.7;
			finalColor.b -= 0.7;
			finalColor.g += 0.7;
			FragColor = finalColor;
		}
		else if (IN.temp_position.x >= a2 && IN.temp_position.x < a3) {
			finalColor.r -= 0.7;
			finalColor.g -= 0.7;
			finalColor.b += 0.7;
			FragColor = finalColor;
		}
		else if (IN.temp_position.x >= a3 && IN.temp_position.x <= a4) {
			finalColor.r += 0.15;
			finalColor.b += 0.15;
			finalColor.g -= 0.7;
			FragColor = finalColor;
		}
		else {
			FragColor = vec4(0, 0, 0, 1);
		}
	}
	else {
		float playerPerc = health * 0.8 - 0.4;
		if (IN.temp_position.x < playerPerc) {
			FragColor = finalColor;
		}
		else {
			FragColor = vec4(0.2, 0.2, 0.2, 1);
		}
	}
}