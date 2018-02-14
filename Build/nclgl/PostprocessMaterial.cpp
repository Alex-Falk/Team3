#include "PostprocessMaterial.h"
#include "../ncltech/GraphicsPipeline.h"
bool PresentToWindowMaterial::Apply()
{
	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Present_To_Window];

	return false;
}
