#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Textures.hpp"

namespace BGE
{
	GLTexture::GLTexture(void)
	{

	}

	GLTexture::~GLTexture(void)
	{
	}

	bool GLTexture::VCreate(TextureType type)
	{
		m_desc.type = type;
		glCreateTextures(ConvertTextureTarget(type), 1, &m_textureID);
		return glIsTexture(m_textureID) == GL_TRUE;
	}

	void GLTexture::VDestroy(void)
	{
		if (glIsTexture(m_textureID))
		{
			glDeleteTextures(1, &m_textureID);
		}
	}

	bool GLTexture::VInit(StrongResourceHandlePtr pHandle, const TextureDesc &kDesc)
	{
		if (pHandle->GetType() != ResourceType::kJPEG)
		{
			BGE_LOG("GL", "Oops");
			return false;
		}



		return true;
	}

	void GLTexture::Bind(std::uint32_t unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(ConvertTextureTarget(m_desc.type), m_textureID);
	}

	TextureType GLTexture::VGetType(void) const
	{
		return m_desc.type;
	}

	std::uint32_t GLTexture::VGetWidth(void) const
	{
		return m_desc.width;
	}

	std::uint32_t GLTexture::VGetHeight(void) const
	{
		return m_desc.height;
	}

	std::uint32_t GLTexture::VGetDepth(void) const
	{
		return m_desc.depth;
	}

	GLenum GLTexture::ConvertTextureTarget(TextureType type)
	{
		switch (type)
		{
		case TextureType::k1D:
			return GL_TEXTURE_1D;
		case TextureType::k2D:
			return GL_TEXTURE_2D;
		case TextureType::k3D:
			return GL_TEXTURE_3D;
		case TextureType::kCubeMap:
			return GL_TEXTURE_CUBE_MAP;
		case TextureType::k2DArray:
			return GL_TEXTURE_2D_ARRAY;
		case TextureType::kCubeMapArray:
			return GL_TEXTURE_CUBE_MAP_ARRAY;
		case TextureType::k2DMultisample:
			return GL_TEXTURE_2D_MULTISAMPLE;
		case TextureType::k2DMultisampleArray:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		case TextureType::kBuffer:
			return GL_TEXTURE_BUFFER;
		default:
			BGE_ASSERT(false && "Invalid texture type");
			return 0;
		}
	}
} // End namespace (BGE)
