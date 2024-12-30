#include "Engine/EngineStd.hpp"
#include "Graphics/ShaderProgram.hpp"

std::optional<GLint> BGE::IShaderProgram::GetUniformLocation(GLuint programID, std::string_view uniformName) noexcept
{
    GLint result = glGetUniformLocation(programID, uniformName.data());
    return (result < 0) ? std::nullopt : std::optional<GLint>(result);
}

BGE::ShaderProgram::ShaderProgram(void)
    : m_programID(0)
{
}

BGE::ShaderProgram::~ShaderProgram(void)
{
    if (VIsValid())
	{
		VDestroy(); // Ensure the program is destroyed
	}
}

bool BGE::ShaderProgram::VCreate(void)
{
    m_programID = glCreateProgram();
	return m_programID != 0;
}

GLuint BGE::ShaderProgram::VGetID(void) const
{
    return m_programID;
}

void BGE::ShaderProgram::VAttachShader(StrongIShaderPtr pShader)
{
	if (!pShader)
	{
		return; // If the passed pointer is null, exit early
	}

	auto findIt = std::ranges::find(m_shaders, pShader);
	if (findIt != m_shaders.end())
	{
		return; // Can't link against the same shader multiple times
	}

	m_shaders.push_back(pShader);
	// Attach the shader to the program
	glAttachShader(m_programID, pShader->VGetID());
}

void BGE::ShaderProgram::VDetachShader(StrongIShaderPtr pShader)
{
	if (!pShader)
	{
		return; // If the passed pointer is null, exit early
	}

	m_shaders.remove_if([&pShader](const StrongIShaderPtr &shader)
	{
		return shader == pShader; // Remove the shader if it matches
	});

	// Detach the shader from the program
	glDetachShader(m_programID, pShader->VGetID());
}

bool BGE::ShaderProgram::VLink(void)
{
    // Link all attached shader objects to this program
	glLinkProgram(m_programID);

	GLint status{};
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		// Get the length of the program info log
		GLint infoLogLength{};
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		// Retrieve the program info log
		std::string infoLog(infoLogLength, '\0');
		GLsizei length{};
		glGetProgramInfoLog(m_programID, infoLogLength, &length, infoLog.data());

		// Log the error message
		BGE_LOG("Graphics", "Shader program link failure: %s", infoLog.c_str());
		return false;
	}
    return true;
}

void BGE::ShaderProgram::VBind(void)
{
    glUseProgram(m_programID);
}

void BGE::ShaderProgram::VDestroy(void)
{
    glDeleteProgram(m_programID);
}

bool BGE::ShaderProgram::VIsValid(void) const
{
    return glIsProgram(m_programID);
}

void BGE::ShaderProgram::SetBool(std::string_view uniformName, bool value)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform1i(m_programID, UniformLocation(uniformName), static_cast<bool>(value));
}

void BGE::ShaderProgram::SetInt(std::string_view uniformName, GLint value)
{
    UpdateUniformLocation(uniformName);
	glProgramUniform1i(m_programID, UniformLocation(uniformName), value);
}

void BGE::ShaderProgram::SetUnsignedInt(std::string_view uniformName, GLuint value)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform1ui(m_programID, UniformLocation(uniformName), value);
}

void BGE::ShaderProgram::SetFloat(std::string_view uniformName, GLfloat value)
{
    UpdateUniformLocation(uniformName);
	glProgramUniform1f(m_programID, UniformLocation(uniformName), value);
}

void BGE::ShaderProgram::SetDouble(std::string_view uniformName, GLdouble value)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform1d(m_programID, UniformLocation(uniformName), value);
}

void BGE::ShaderProgram::SetVec2(std::string_view uniformName, const glm::vec2   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform2fv(m_programID, UniformLocation(uniformName), 1, &kValue[0]);
}

void BGE::ShaderProgram::SetVec3(std::string_view uniformName, const glm::vec3   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform3fv(m_programID, UniformLocation(uniformName), 1, &kValue[0]);
}

void BGE::ShaderProgram::SetVec4(std::string_view uniformName, const glm::vec4   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniform4fv(m_programID, UniformLocation(uniformName), 1, &kValue[0]);
}

void BGE::ShaderProgram::SetMat2(std::string_view uniformName, const glm::mat2   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix2fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat2(std::string_view uniformName, const glm::mat2x3 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix2x3fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat2(std::string_view uniformName, const glm::mat2x4 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix2x4fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat3(std::string_view uniformName, const glm::mat3   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix3fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat3(std::string_view uniformName, const glm::mat3x2 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix3x2fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat3(std::string_view uniformName, const glm::mat3x4 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix3x4fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat4(std::string_view uniformName, const glm::mat4   &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix4fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat4(std::string_view uniformName, const glm::mat4x2 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix4x2fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::SetMat4(std::string_view uniformName, const glm::mat4x3 &kValue)
{
	UpdateUniformLocation(uniformName);
	glProgramUniformMatrix4x3fv(m_programID, UniformLocation(uniformName), 1, GL_FALSE, &kValue[0][0]);
}

void BGE::ShaderProgram::UpdateUniformLocation(std::string_view uniformName)
{
	std::string name(uniformName);
	if (m_uniformLocations.find(name) == m_uniformLocations.end())
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		m_uniformLocations[name] = *result;
	}
}

int BGE::ShaderProgram::UniformLocation(std::string_view uniformName)
{
	return m_uniformLocations[std::string(uniformName)];
}
