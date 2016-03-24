#include "stdafx.hpp"
#include "SkyboxMaterial.hpp"

#include "../../Graphics/ShaderData.hpp"
#include "../../Graphics/MeshFilter.hpp"
#include "../../Graphics/TextureData.hpp"

#include "../../Content/CubeMapLoader.hpp"

SkyboxMaterial::SkyboxMaterial(string assetFile):
	Material("Resources/Shaders/FwdSkyboxShader.glsl"),
	m_AssetFile(assetFile)
{
	m_LayoutFlags = VertexFlags::POSITION;
	m_DrawForward = true;
}
SkyboxMaterial::~SkyboxMaterial()
{
}

void SkyboxMaterial::LoadTextures()
{
	CubeMapLoader* pCmL = ContentManager::GetLoader<CubeMapLoader, CubeMap>();
	pCmL->UseSrgb(true);
	m_pMap = ContentManager::Load<CubeMap>(m_AssetFile);
}
void SkyboxMaterial::AccessShaderAttributes()
{
}
void SkyboxMaterial::UploadDerivedVariables()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(m_Shader->GetProgram());
	GLuint uMapName = glGetUniformLocation(m_Shader->GetProgram(), "skybox");
	glUniform1i(uMapName, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pMap->GetHandle());
}