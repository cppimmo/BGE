#include "Engine/EngineStd.hpp"
#include "Graphics/D3D11/D3DShaderProgram.hpp"

namespace BGE
{
	D3DShaderProgram::D3DShaderProgram(void)
	{
	}

	D3DShaderProgram::~D3DShaderProgram(void)
	{
		VDestroy();
	}

	void BGE::D3DShaderProgram::VBind(void)
	{
	}

	void BGE::D3DShaderProgram::VUnbind(void)
	{
	}

	bool BGE::D3DShaderProgram::VCreate(void)
	{
		return true;
	}
	
	GLuint BGE::D3DShaderProgram::VGetID(void) const
	{
		return GLuint();
	}
	
	void BGE::D3DShaderProgram::VAttachShader(StrongIShaderPtr pShader)
	{
	}
	
	void BGE::D3DShaderProgram::VDetachShader(StrongIShaderPtr pShader)
	{
	}
	
	bool BGE::D3DShaderProgram::VLink(void)
	{
		return true;
	}
	
	void BGE::D3DShaderProgram::VDestroy(void)
	{
	}
	
	bool BGE::D3DShaderProgram::VIsValid(void) const
	{
		return false;
	}
	
	void BGE::D3DShaderProgram::VSetBool(std::string_view uniformName, bool value)
	{
	}
	
	void BGE::D3DShaderProgram::VSetInt(std::string_view uniformName, int value)
	{
	}
	
	void BGE::D3DShaderProgram::VSetUnsignedInt(std::string_view uniformName, unsigned int value)
	{
	}
	
	void BGE::D3DShaderProgram::VSetFloat(std::string_view uniformName, float value)
	{
	}
	
	void BGE::D3DShaderProgram::VSetDouble(std::string_view uniformName, double value)
	{
	}
	
	void BGE::D3DShaderProgram::VSetVec2(std::string_view uniformName, const glm::vec2 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetVec3(std::string_view uniformName, const glm::vec3 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetVec4(std::string_view uniformName, const glm::vec4 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetMat2(std::string_view uniformName, const glm::mat2 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetMat3(std::string_view uniformName, const glm::mat3 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetMat4(std::string_view uniformName, const glm::mat4 &kValue)
	{
	}
	
	void BGE::D3DShaderProgram::VSetStruct(std::string_view uniformName, const UniformStruct &kStruct, const void *pStructData)
	{
	}
} // End namespace (BGE)
