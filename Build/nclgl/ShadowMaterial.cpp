
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
#include "ShadowMaterial.h"
#include "../ncltech/GraphicsPipeline.h"

bool ShadowMaterial::Apply()
{
	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Shadow];

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uShadowTransform[0]"), SHADOWMAP_NUM, GL_FALSE, (float*)&graphicsPipeline->GetShadowProjViewMatrices()[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&renderNode->GetWorldTransform());
	return true;
}
