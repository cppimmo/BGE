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
    auto result = GetUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result)
	{
		glProgramUniform1i(m_programID, *result, static_cast<bool>(value));
	}
}

void BGE::ShaderProgram::SetInt(std::string_view uniformName, GLint value)
{
    auto result = GetUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result)
	{
		glProgramUniform1i(m_programID, *result, value);
	}
}

void BGE::ShaderProgram::SetUnsignedInt(std::string_view uniformName, GLuint value)
{
    auto result = GetUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result)
	{
		glProgramUniform1ui(m_programID, *result, value);
	}
}

void BGE::ShaderProgram::SetFloat(std::string_view uniformName, GLfloat value)
{
    auto result = GetUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result)
	{
		glProgramUniform1f(m_programID, *result, value);
	}
}

void BGE::ShaderProgram::SetDouble(std::string_view uniformName, GLdouble value)
{
    auto result = GetUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result)
	{
		glProgramUniform1d(m_programID, *result, value);
	}
}
