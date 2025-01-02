#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Shaders.hpp"

namespace BGE
{
	GLShader::GLShader(void)
		: m_shaderID(0)
	{
	}

	GLShader::~GLShader(void)
	{
		VDestroy();
	}

	bool GLShader::VCompile(std::string_view source)
	{
		return GLShader::Compile(m_shaderID, source);
	}

	bool GLShader::VCompile(StrongResourceHandlePtr pResourceHandle)
	{
		// TODO: Add some error checking here to ensure the handle is GLSL.
		std::string source = pResourceHandle->GetExtraData()->VGetExtraData();
		return VCompile(source);
	}

	bool GLShader::VCompileBinary(StrongResourceHandlePtr pResourceHandle, std::string_view entryPoint)
	{
		//glShaderBinary(1, &m_shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, nullptr, 0);

		// Check the shader compilation status
		GLint status{};
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			// Get the length of the shader info log
			GLint infoLogLength{};
			glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Retrieve the shader info log
			std::string infoLog(infoLogLength, '\0');
			GLsizei length{};
			glGetShaderInfoLog(m_shaderID, infoLogLength, &length, infoLog.data());

			// Log the error message
			BGE_LOG("Graphics", "Shader compilation failed: %s", infoLog.c_str());
			return false;
		}
	}

	GLuint GLShader::VGetID(void) const
	{
		return m_shaderID;
	}

	void GLShader::VDestroy(void)
	{
		glDeleteShader(m_shaderID);
	}

	bool GLShader::VIsValid(void) const
	{
		return glIsShader(m_shaderID);
	}

	bool GLShader::Compile(GLuint shaderID, std::string_view source)
	{
		const char *pSourceData = source.data();
		const GLint kSourceLength = static_cast<GLint>(source.size());

		// Attach the source to the shader
		glShaderSource(shaderID, 1, &pSourceData, &kSourceLength);
		glCompileShader(shaderID); // Compile the shader

		// Check the shader compilation status
		GLint status{};
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			// Get the length of the shader info log
			GLint infoLogLength{};
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Retrieve the shader info log
			std::string infoLog(infoLogLength, '\0');
			GLsizei length{};
			glGetShaderInfoLog(shaderID, infoLogLength, &length, infoLog.data());

			// Log the error message
			BGE_LOG("Graphics", "Shader compilation failed: %s", infoLog.c_str());
			return false;
		}
		return true;
	}

	StrongIShaderPtr GLShaderFactory::VCreateVertexShader(void) const
	{
		return std::make_shared<GLVertexShader>();
	}

	StrongIShaderPtr GLShaderFactory::VCreateTessControlShader(void) const
	{
		return std::make_shared<GLTessControlShader>();
	}

	StrongIShaderPtr GLShaderFactory::VCreateTessEvalShader(void) const
	{
		return std::make_shared<GLTessEvalShader>();
	}

	StrongIShaderPtr GLShaderFactory::VCreateGeometryShader(void) const
	{
		return std::make_shared<GLGeometryShader>();
	}

	StrongIShaderPtr GLShaderFactory::VCreateFragmentShader(void) const
	{
		return std::make_shared<GLFragmentShader>();
	}

	StrongIShaderPtr GLShaderFactory::VCreateComputeShader(void) const
	{
		return std::make_shared<GLComputeShader>();
	}

	bool GLVertexShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_VERTEX_SHADER);
		return m_shaderID != 0;
	}

	bool GLTessControlShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		return m_shaderID != 0;
	}

	bool GLTessEvalShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		return m_shaderID != 0;
	}

	bool GLGeometryShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
		return m_shaderID != 0;
	}

	bool GLFragmentShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		return m_shaderID != 0;
	}

	bool GLComputeShader::VCreate(void)
	{
		m_shaderID = glCreateShader(GL_COMPUTE_SHADER);
		return m_shaderID != 0;
	}
} // End namespace (BGE)
