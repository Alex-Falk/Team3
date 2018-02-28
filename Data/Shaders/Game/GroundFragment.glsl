
/*****************************************************************************
:;-"""-::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
(      .)::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
        )-:::::::::::::::::::::::::::::::::::::::::::::::.::..:... .. .
      -'   )-"-:::::::::::::::::::::::::::::::::::::::.::..:... ..  .
 ___________)______________          _____
|                         \          \ U \__      _______
|        Yesheng Su        \__________\   \/_______\___\_____________
|        10/02/2018        /          < /_/   ..................... ^`-._
|_________________________/            `-----------,----,--------------'
                      )                          _/____/_
-.                .    ):::::::::::::::::::::::::::::.::..:... ..  .
  )--.__..--"-.__,'---':::::::::::::::::::::::::::::::::.::..:... ..  .
-':::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
:::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
															
*****************************************************************************/
//Add MRT - Jeff
#version 330 core
#define SHADOWMAP_NUM  4

//Per object
uniform float		smoothness;
uniform samplerCube	cubeTex;

uniform sampler2D	uPathTex;
uniform sampler2D  	uDiffuseTex;
uniform vec4		uColor;

//Constant Per Frame
uniform vec3  		uCameraPos;
uniform vec3		uAmbientColor;
uniform vec3		uLightDirection;
uniform float		uSpecularFactor;

//Shadow

uniform sampler2DArrayShadow uShadowTex;
uniform vec2  		uShadowSinglePixel;
uniform float 		uNormalizedFarPlanes[SHADOWMAP_NUM - 1]; 
uniform mat4 		uShadowTransform[SHADOWMAP_NUM];

in Vertex	{
	smooth vec3 	 worldPos;
	smooth vec2 	 texCoord;
	smooth vec3 	 normal;
} IN;

layout(location = 0) out vec4 OutFrag;
layout(location = 1) out vec4 BrightColor;

const float NORMAL_BIAS = 0.003f;
const float RAW_BIAS 	= 0.00025f;

float DoShadowTest(vec3 tsShadow, int tsLayer, vec2 pix)
{	
	vec4 tCoord;
	tCoord.xyw = tsShadow;	//I don't know why...
	tCoord.z = tsLayer;

	//Far
	if (tsLayer > 0)
	{
		/* Far away shadowmaps
		Fast single shadow sample 
		*/
		return texture(uShadowTex, tCoord);
	}
	else
	{
		/* Near shadowmaps (or in case of looking in the light dir, the centre of the screen)
		PCF filtering
		   - Takes a 4x4 sample around each pixel and averages the result, bluring the edges of shadows
		   - Requires 16 linearly interpolated samples which can be quite expensive
		   - Consider looking into exponential shadow mapping as a nicer looking and faster way to achieve soft shadowing.
		 */
		float shadow = 0.0f;
		for (float y = -1.5f; y <= 1.5f; y += 1.0f)
			for (float x = -1.5f; x <= 1.5f; x += 1.0f)
				shadow += texture(uShadowTex, tCoord + vec4(pix.x * x, pix.y * y, 0, 0));
			
		return shadow / 16.0f;
	}
}

void main(void)	{
	vec3 normal 	= normalize(IN.normal);
	vec4 texColor  	= texture(uDiffuseTex, IN.texCoord);
	vec4 color 		= uColor * texColor;

//Shadow Calculations
	vec4 shadowWsPos = vec4(IN.worldPos + normal * NORMAL_BIAS, 1.0f);
	
	float shadow = 1.0f;
	for (int layerIdx = 0; layerIdx < SHADOWMAP_NUM; layerIdx++)
	{
		//We don't need to divide by w for orthographic projections
		vec4 hcsShadow = uShadowTransform[layerIdx] * shadowWsPos;
	
		if (abs(hcsShadow.x) <= 1.0f && abs(hcsShadow.y) <= 1.0f)
		{
			hcsShadow.z -= RAW_BIAS;
			hcsShadow.xyz = hcsShadow.xyz * 0.5f + 0.5f;

			shadow = DoShadowTest(hcsShadow.xyz, layerIdx, uShadowSinglePixel);
			break;
		}
	}
	
//Lighting Calculations
	vec3 viewDir 		= normalize(uCameraPos - IN.worldPos);
	vec3 halfDir 		= normalize(viewDir - uLightDirection);
	float rFactor       = max(0.0, dot(halfDir , normal ));
	
	float dFactor       = max(0.0, dot(-uLightDirection , normal)) ;
    float sFactor       = pow(rFactor , uSpecularFactor );
	   
//Color Computations
	const float specIntensity = 0.4f;
	
	//Quick hack to approximate specular colour of an object, assuming the light colour is white
	vec3 specColor = min(color.rgb + vec3(0.5f), vec3(1)); 
	
//Output Final Colour
	vec3 diffuse  = color.rgb * dFactor * shadow;
	vec3 specular = specColor * sFactor * shadow * specIntensity;
	
	//Combine lighting
	vec4 finalLightColor;
	finalLightColor.xyz 	= diffuse + specular + color.rgb* uAmbientColor;
	finalLightColor.a 		= color.a;

	vec3 toWPos = normalize(IN.worldPos - uCameraPos);
	vec4 reflection = texture(cubeTex, reflect(toWPos, normalize(IN.normal)))*smoothness;
	reflection = reflection * vec4(diffuse,0.0f);
	finalLightColor = finalLightColor + reflection;

	vec2 coord = vec2(-IN.texCoord.x, -IN.texCoord.y);
	vec4 pathColor = texture(uPathTex, coord);
	vec3 up = vec3(0, 1, 0);
	vec4 FinalColor;
	if(length((normal - up)) < 0.1)
		FinalColor = pathColor + finalLightColor;
	else
		FinalColor = finalLightColor;
	OutFrag = FinalColor;
	float brightness = dot(FinalColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness >= 0.2) {
		//BrightColor = FinalColor;
	}
}