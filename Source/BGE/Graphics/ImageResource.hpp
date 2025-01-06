/*******************************************************************************
 * @file   ImageResource.hpp
 * @author Brian Hoffpauir
 * @date   12.28.2024
 * @brief  .
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#ifndef _BGE_IMAGERESOURCE_HPP_
#define _BGE_IMAGERESOURCE_HPP_

#include "Resources/Resource.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceLoader.hpp"

namespace BGE
{
	class ImageResourceExtraData; // Forward declare
	class BMPResourceLoader;  // Forward declare
	class JPEGResourceLoader; // Forward declare
	class PNGResourceLoader;  // Forward declare
	class TGAResourceLoader;  // Forward declare
	BGE_DECLARE_PTR(ImageResourceExtraData);
	BGE_DECLARE_PTR(BMPResourceLoader);
	BGE_DECLARE_PTR(JPEGResourceLoader);
	BGE_DECLARE_PTR(PNGResourceLoader);
	BGE_DECLARE_PTR(TGAResourceLoader);

	/**
	 * @brief .
	 */
	inline constexpr std::size_t kMAX_TEXTURE_MIPS = 14u;

	enum struct ImageFormat
	{
		kUndefined,
		kRGB,
		kRGBA,
		kDepth
	};

	enum struct ImageType
	{
		kUnsignedByte,
		kByte,
		kUnsignedShort,
		kShort,
		kUnsignedInt,
		kInt,
		kFloat
	};

	struct MipData
	{
		int width;
		int height;
		int depth;
		std::size_t mipStride;
		void *pData;
	};

	struct ImageData
	{
		enum struct Target
		{
			kTexture1D, /**< . */
			kTexture2D, /**< . */
			kTexture3D, /**< . */
			kTextureCubeMap, /**< . */
			kTexture2DArray, /**< . */
			kTextureCubeMapArray, /**< . */
			kTexture2DMultisample, /**< . */
			kTexture2DMultisampleArray, /**< . */
			kTextureBuffer /**< . */
		};

		Target target;
		ImageFormat internalFormat;
		ImageFormat memoryFormat;
		ImageType memoryType;
		std::array<int, 4> swizzle;
		int mipLevels;
		int slices;
		std::size_t sliceStride;
		std::size_t totalDataSize;
		std::vector<MipData> mips;
	};
#if 0
	/**
	 * @brief .
	 */
	struct ImageMipData
	{
		GLsizei width;  //!< Width of this mipmap level
		GLsizei height; //!< Height of this mipmap level
		GLsizei depth;  //!< Depth of the mipmap level
		GLsizeiptr mipStride; //!< Distance between mip levels in memory
		GLvoid *pData; //!< Pointer to the image data
	};

	/**
	 * @brief .
	 */
	struct ImageData
	{
		GLenum target; //!< Texture target (2D, cube map, etc.)
		GLenum internalFormat; //!< Recommended internal format
		GLenum format; //!< Format in memory
		GLenum type; //!< Type in memory (GL_RGB, etc.)
		std::array<GLenum, 4> swizzle; //!< Swizzle for RGBA
		GLsizei mipLevels; //!< Number of present mipmap levels
		GLsizei slices; //!< Number of slices (for arrays)
		GLsizeiptr sliceStride; //!< Distance between slices of an array texture
		GLsizeiptr totalDataSize; //!< Total data allocated for texture
		std::array<ImageMipData, kMAX_TEXTURE_MIPS> mip; //!< Actual mipmap data
	};
#endif
	/**
	 * @brief .
	 */
	class ImageResourceExtraData final : public IResourceExtraData
	{
	private:
		ImageData m_data;
	public:
		ImageResourceExtraData(const ImageData &kData);
		~ImageResourceExtraData(void) override;
		// IResourceExtraData's interface:
		virtual std::string VGetExtraData(void) override;

		const ImageData &GetImageData(void) const;
	};

	/**
	 * @brief .
	 */
	class BMPResourceLoader : public IResourceLoader
	{
	public:
		BMPResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief .
	 */
	class JPEGResourceLoader : public IResourceLoader
	{
	public:
		JPEGResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief .
	 */
	class PNGResourceLoader : public IResourceLoader
	{
	public:
		PNGResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief .
	 */
	class TGAResourceLoader : public IResourceLoader
	{
	public:
		TGAResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual std::string VGetPattern(void) const override;
		virtual ResourceType VGetType(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_IMAGERESOURCE_HPP_ */
