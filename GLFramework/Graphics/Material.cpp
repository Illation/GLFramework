#include "Material.hpp"

#include "../Content/ShaderLoader.hpp"
#include "../Base\Context.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Content/ContentManager.hpp"
#include "ShaderData.hpp"
#include "MeshFilter.hpp"

#define CAMERA Context::GetInstance()->pCamera

Material::Material(std::string shaderFile) :
	m_ShaderFile(shaderFile)
{
}
Material::~Material()
{
}

void Material::Initialize()
{
	if (!m_IsInitialized)
	{
		//Load Shader
		m_Shader = ContentManager::Load<ShaderData>(m_ShaderFile);

		//Jup (maybe temporary with texture manager)
		LoadTextures();

		//Get Access to uniforms
		m_UniMatModel = glGetUniformLocation(m_Shader->GetProgram(), "model");
		m_UniMatWVP = glGetUniformLocation(m_Shader->GetProgram(), "worldViewProj");
		AccessShaderAttributes();

		m_IsInitialized = true;
	}
}

void Material::UploadVariables(glm::mat4 matModel)
{
	glUseProgram(m_Shader->GetProgram());
	//Upload matrices
	glUniformMatrix4fv(m_UniMatModel, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(m_UniMatWVP, 1, GL_FALSE, glm::value_ptr(CAMERA->GetViewProj()));

	UploadDerivedVariables();
}

void Material::SpecifyInputLayout()
{
	unsigned stride = 0;
	for (auto it = MeshFilter::LayoutAttributes.begin(); it != MeshFilter::LayoutAttributes.end(); ++it)
	{
		if (m_LayoutFlags & it->first) stride += it->second.dataSize;
	}
	unsigned startPos = 0;
	for (auto it = MeshFilter::LayoutAttributes.begin(); it != MeshFilter::LayoutAttributes.end(); ++it)
	{
		if (m_LayoutFlags & it->first)
		{
			GLint attrib = glGetAttribLocation(m_Shader->GetProgram(), it->second.name.c_str());
			glEnableVertexAttribArray(attrib);
			glVertexAttribPointer(attrib, it->second.dataSize, it->second.dataType, GL_FALSE,
				stride * sizeof(GLfloat), (void*)(startPos * sizeof(GLfloat)));
			startPos += it->second.dataSize;
		}
	}
}