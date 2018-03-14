
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
#include "StandardMaterial.h"
#include "../ncltech/GraphicsPipeline.h"
#include "../ncltech/TextureManager.h"
bool StandardMaterial::Apply()
{

	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	TextureManager* textureManager = TextureManager::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Forward_Lighting];

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&graphicsPipeline->GetProjViewMtx());

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uDiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, renderNode->GetTexture());

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uCameraPos"), 1, (float*)&graphicsPipeline->GetCamera()->GetPosition());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uAmbientColor"), 1, (float*)&graphicsPipeline->GetAmbientColor());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uLightDirection"), 1, (float*)&graphicsPipeline->GetLightDirection());
	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uSpecularFactor"), 1, &graphicsPipeline->GetSpecularFactor());

	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uNormalizedFarPlanes[0]"), SHADOWMAP_NUM - 1, (float*)&graphicsPipeline->GetNormalizedFarPlanes()[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uShadowTransform[0]"), SHADOWMAP_NUM, GL_FALSE, (float*)&graphicsPipeline->GetShadowProjViewMatrices()[0]);
	glUniform2f(glGetUniformLocation(shader->GetProgram(), "uShadowSinglePixel"), 1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uShadowTex"), 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, graphicsPipeline->GetShadowTex());

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "smoothness"), renderNode->GetSmoothness());
	glActiveTexture(GL_TEXTURE6);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager::Instance()->GetTexture(TEXTURETYPE::Sky_Box));

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&renderNode->GetWorldTransform());
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "uColor"), 1, (float*)&renderNode->GetColor());
	return true;
}

bool DrawPathMaterial::Apply()
{
	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Draw_Path];
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&projViewMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&renderNode->GetWorldTransform());
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "uColor"), 1, (float*)&renderNode->GetColor());
	return true;
}

bool GroundMaterial::Apply()
{
	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	TextureManager* textureManager = TextureManager::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Ground];

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&graphicsPipeline->GetProjViewMtx());

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uDiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, renderNode->GetTexture());

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uCameraPos"), 1, (float*)&graphicsPipeline->GetCamera()->GetPosition());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uAmbientColor"), 1, (float*)&graphicsPipeline->GetAmbientColor());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uLightDirection"), 1, (float*)&graphicsPipeline->GetLightDirection());
	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uSpecularFactor"), 1, &graphicsPipeline->GetSpecularFactor());

	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uNormalizedFarPlanes[0]"), SHADOWMAP_NUM - 1, (float*)&graphicsPipeline->GetNormalizedFarPlanes()[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uShadowTransform[0]"), SHADOWMAP_NUM, GL_FALSE, (float*)&graphicsPipeline->GetShadowProjViewMatrices()[0]);
	glUniform2f(glGetUniformLocation(shader->GetProgram(), "uShadowSinglePixel"), 1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uShadowTex"), 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, graphicsPipeline->GetShadowTex());

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&renderNode->GetWorldTransform());
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "uColor"), 1, (float*)&renderNode->GetColor());

	glActiveTexture(GL_TEXTURE5);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uPathTex"), 5);
	glBindTexture(GL_TEXTURE_2D, graphicsPipeline->GetPathTex());
	glUniform2f(glGetUniformLocation(shader->GetProgram(), "uGroundSize"), graphicsPipeline->GetGroundSize().x, graphicsPipeline->GetGroundSize().y);

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "smoothness"), renderNode->GetSmoothness());
	glActiveTexture(GL_TEXTURE6);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager::Instance()->GetTexture(TEXTURETYPE::Sky_Box));

	return true;
}

bool ChangeColorMaterial::Apply()
{
	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	TextureManager* textureManager = TextureManager::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Forward_Lighting];

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&graphicsPipeline->GetProjViewMtx());

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uDiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, renderNode->GetTexture());

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uCameraPos"), 1, (float*)&graphicsPipeline->GetCamera()->GetPosition());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uAmbientColor"), 1, (float*)&graphicsPipeline->GetAmbientColor());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uLightDirection"), 1, (float*)&graphicsPipeline->GetLightDirection());
	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uSpecularFactor"), 1, &graphicsPipeline->GetSpecularFactor());

	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uNormalizedFarPlanes[0]"), SHADOWMAP_NUM - 1, (float*)&graphicsPipeline->GetNormalizedFarPlanes()[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uShadowTransform[0]"), SHADOWMAP_NUM, GL_FALSE, (float*)&graphicsPipeline->GetShadowProjViewMatrices()[0]);
	glUniform2f(glGetUniformLocation(shader->GetProgram(), "uShadowSinglePixel"), 1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uShadowTex"), 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, graphicsPipeline->GetShadowTex());

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "smoothness"), renderNode->GetSmoothness());
	glActiveTexture(GL_TEXTURE6);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager::Instance()->GetTexture(TEXTURETYPE::Sky_Box));

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&renderNode->GetWorldTransform());
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "uColor"), 1, (float*)&renderNode->GetColor());
	return true;
}
