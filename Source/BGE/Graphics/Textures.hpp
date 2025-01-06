#ifndef _BGE_TEXTURES_HPP_
#define _BGE_TEXTURES_HPP_

#include "Resources/ResourceHandle.hpp"
#include "Graphics/Renderer.hpp"

namespace BGE
{
	class ITexture; // Forward declare
	BGE_DECLARE_PTR(ITexture);

	//! .
	enum struct TextureType
	{
		k1D, /**< . */
		k2D, /**< . */
		k3D, /**< . */
		kCubeMap, /**< . */
		k2DArray, /**< . */
		kCubeMapArray, /**< . */
		k2DMultisample, /**< . */
		k2DMultisampleArray, /**< . */
		kBuffer /**< . */
	};

	//! .
	enum struct TextureFormat
	{
		kRGBA8,
		kRGBA16F,
		kRG32F,
		kDepth24Stencil8,
	};

	//! Descriptor for textures.
	struct TextureDesc
	{
		TextureType type;
		TextureFormat format;
		std::uint32_t width;
		std::uint32_t height = 1; //!< Default for non-2D textures.
		std::uint32_t depth = 1; //!< Default for non-3D textures
		std::uint32_t levels = 1; //!< Number of mip levels.
		std::uint32_t samples = 1; //!< For multisample textures.
		bool bGenerateMipmaps = false;
	};

	class ITexture
	{
	public:
		virtual ~ITexture(void) = default;
		// Interface:
		virtual bool VCreate(TextureType type) = 0;
		virtual void VDestroy(void) = 0;
		virtual bool VInit(StrongResourceHandlePtr pHandle, const TextureDesc &kDesc) = 0;
		virtual void Bind(std::uint32_t unit) const = 0;
		virtual TextureType VGetType(void) const = 0;
		virtual std::uint32_t VGetWidth(void) const = 0;
		virtual std::uint32_t VGetHeight(void) const = 0;
		virtual std::uint32_t VGetDepth(void) const = 0;
	};

	//class TextureFactory
	//{
	//public:
	//	StrongITexturePtr VCreateTexture(RendererImpl impl)
	//	{
	//		switch (impl)
	//		{
	//			case RendererImpl::kOpenGL:
	//				return std::make_shared<
	//			default:
	//				return nullptr;
	//		}
	//	}
	//};
} // End namespace (BGE)

#endif /* !_BGE_TEXTURES_HPP_ */
