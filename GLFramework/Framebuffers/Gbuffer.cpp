#include "stdafx.hpp"
#include "Gbuffer.hpp"

#include "../Graphics/ShaderData.hpp"
#include "../SceneGraph/AbstractScene.hpp"
#include "../Components/LightComponent.hpp"
#include "../Graphics/TextureData.hpp"
#include "../Prefabs/Skybox.hpp"

Gbuffer::Gbuffer(bool demo):
	FrameBuffer(demo?
		"Shaders/PostBufferDisplay.glsl":
		"Shaders/PostDeferredComposite.glsl", 
		GL_FLOAT, 3)
{
}
Gbuffer::~Gbuffer()
{
}

void Gbuffer::AccessShaderAttributes()
{
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texPosAO"), 0);
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texNormMetSpec"), 1);
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texBaseColRough"), 2);

	m_uCamPos = glGetUniformLocation(m_pShader->GetProgram(), "camPos");
}
void Gbuffer::UploadDerivedVariables()
{
	if (SCENE->SkyboxEnabled())
	{
		glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texIrradiance"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SCENE->GetEnvironmentMap()->GetIrradianceHandle());

		glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texEnvRadiance"), 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SCENE->GetEnvironmentMap()->GetRadianceHandle());
		
		glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texBRDFLUT"), 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, SCENE->GetEnvironmentMap()->GetBrdfLutHandle());

		glUniform1f(glGetUniformLocation(m_pShader->GetProgram(), "MAX_REFLECTION_LOD"), (GLfloat)SCENE->GetEnvironmentMap()->GetNumMipMaps());
	}

	glm::vec3 cPos = CAMERA->GetTransform()->GetPosition();
	glUniform3f(m_uCamPos, cPos.x, cPos.y, cPos.z);
}