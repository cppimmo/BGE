#ifndef _BGE_GL_TEXTURES_HPP_
#define _BGE_GL_TEXTURES_HPP_

#include "Graphics/Textures.hpp"

namespace BGE
{
	class GLTexture; // Forward declare
	BGE_DECLARE_PTR(GLTexture);

	/**
	 * @brief .
	 */
	class GLTexture final : public ITexture
	{
	private:
		GLuint m_textureID;
		TextureDesc m_desc;
	public:
		GLTexture(void);
		~GLTexture(void) override;

		virtual bool VCreate(TextureType type) override;
		virtual void VDestroy(void) override;
		virtual bool VInit(StrongResourceHandlePtr pHandle, const TextureDesc &kDesc) override;
		virtual void Bind(std::uint32_t unit) const override;
		virtual TextureType VGetType(void) const override;
		virtual std::uint32_t VGetWidth(void) const override;
		virtual std::uint32_t VGetHeight(void) const override;
		virtual std::uint32_t VGetDepth(void) const override;
	private:
		static GLenum ConvertTextureTarget(TextureType type);
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_TEXTURES_HPP_ */
