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
#include "Resources/ResourceLoader.hpp"

namespace BGE
{
	class BMPResourceLoader;  // Forward declare
	class JPEGResourceLoader; // Forward declare
	class PNGResourceLoader;  // Forward declare
	class TGAResourceLoader;  // Forward declare
	BGE_DECLARE_PTR(BMPResourceLoader);
	BGE_DECLARE_PTR(JPEGResourceLoader);
	BGE_DECLARE_PTR(PNGResourceLoader);
	BGE_DECLARE_PTR(TGAResourceLoader);

	/**
	 * @brief .
	 */
	class BMPResourceLoader : public IResourceLoader
	{
		std::string m_pattern = "*.bmp";
	public:
		BMPResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
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
		std::string m_pattern = "*.jpeg";
	public:
		JPEGResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
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
		std::string m_pattern = "*.png";
	public:
		PNGResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
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
		std::string m_pattern = "*.tga";
	public:
		TGAResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_IMAGERESOURCE_HPP_ */
