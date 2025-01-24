#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Shaders.hpp"
#include "Shaders.hpp"
#include "Shaders.hpp"
#include "Shaders.hpp"
#include "Shaders.hpp"
#include "Shaders.hpp"

namespace BGE
{
	GLShader::GLShader(ShaderType shaderType)
		: IShader(shaderType), m_shaderID(0)
	{
	}

	GLShader::~GLShader(void)
	{
		VDestroy();
	}

	bool GLShader::VCreate(void)
	{
		switch (m_type)
		{
		case ShaderType::kVertex:
			m_shaderID = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::kHull:
			m_shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case ShaderType::kDomain:
			m_shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case ShaderType::kGeometry:
			m_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderType::kPixel:
			m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::kCompute:
			m_shaderID = glCreateShader(GL_COMPUTE_SHADER);
			break;
		default:
			BGE_ASSERT(false && "Invalid shader type");
			break;
		}

		return glIsShader(m_shaderID);
	}

	bool GLShader::VCompile(StrongResourceHandlePtr pResourceHandle)
	{
		BGE_ASSERT(pResourceHandle->GetType() == ResourceType::kGLSL);
		// TODO: Add some error checking here to ensure the handle is GLSL.
		std::string source = pResourceHandle->GetExtraData()->VGetExtraData();
		return Compile(m_shaderID, source);
	}

	void *GLShader::VGetBlob(void) const
	{
		return (void *)&m_shaderID;
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
		return std::make_shared<GLShader>(ShaderType::kVertex);
	}

	StrongIShaderPtr GLShaderFactory::VCreateHullShader(void) const
	{
		return std::make_shared<GLShader>(ShaderType::kHull);
	}

	StrongIShaderPtr GLShaderFactory::VCreateDomainShader(void) const
	{
		return std::make_shared<GLShader>(ShaderType::kDomain);
	}

	StrongIShaderPtr GLShaderFactory::VCreateGeometryShader(void) const
	{
		return std::make_shared<GLShader>(ShaderType::kGeometry);
	}

	StrongIShaderPtr GLShaderFactory::VCreatePixelShader(void) const
	{
		return std::make_shared<GLShader>(ShaderType::kPixel);
	}

	StrongIShaderPtr GLShaderFactory::VCreateComputeShader(void) const
	{
		return std::make_shared<GLShader>(ShaderType::kCompute);
	}
} // End namespace (BGE)
