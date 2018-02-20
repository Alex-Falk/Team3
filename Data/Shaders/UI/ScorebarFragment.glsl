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
// 
//////////////////////////////////////////////////////////////////////////////////////

#version 330 core

uniform float player1;
uniform float player2;
uniform float player3;
uniform float player4;

in Vertex{
	vec4 temp_position;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	vec4 player1Color = vec4(1, 0, 0, 1);
	vec4 player2Color = vec4(0, 1, 0, 1);
	vec4 player3Color = vec4(0, 0, 1, 1);
	vec4 player4Color = vec4(0.5, 0.5, 0.5, 1);

	if (IN.temp_position.x < player1) {
		gl_FragColor = player1Color;
	}
	else if (IN.temp_position.x >= player1 && IN.temp_position.x < player2) {
		gl_FragColor = player2Color;
	}
	else if (IN.temp_position.x >= player2 && IN.temp_position.x < player3) {
		gl_FragColor = player3Color;
	}
	else if (IN.temp_position.x >= player3 && IN.temp_position.x <= player4) {
		gl_FragColor = player4Color;
	}
	else {
		gl_FragColor = vec4(0, 0, 0, 1);
	}

	//if (IN.temp_position.x > 0) {
	//	gl_FragColor = player1Color;
	//}
}