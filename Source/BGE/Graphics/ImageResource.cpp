/*******************************************************************************
 * @file   ImageResource.cpp
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
#include "Engine/EngineStd.hpp"
#include "Graphics/ImageResource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace BGE
{
	const std::string &BMPResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool BMPResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool BMPResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool BMPResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t BMPResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool BMPResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		int width, height, channels;

		// Load the BMP image data
		unsigned char *pImageData = stbi_load_from_memory(
			reinterpret_cast<unsigned char *>(pRawBuffer),
			static_cast<int>(size),
			&width,
			&height,
			&channels,
			0 // Keep the original number of channels
		);

		if (!pImageData)
		{
			// Log an error if the image couldn't be loaded
			BGE_ERROR("Failed to load BMP image: %s", stbi_failure_reason());
			return false;
		}

		// Use the loaded image data (width, height, channels, imageData)
		// For example, pass it to the resource handle (assuming it supports setting data)
		//pResourceHandle->SetData(pImageData, width, height, channels);

		// Free the image data after we're done
		stbi_image_free(pImageData);

		return true;
	}

	const std::string &JPEGResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool JPEGResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool JPEGResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool JPEGResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t JPEGResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool JPEGResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		return true;
	}

	const std::string &PNGResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool PNGResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool PNGResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool PNGResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t PNGResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool PNGResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		return true;
	}

	const std::string &TGAResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool TGAResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool TGAResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool TGAResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t TGAResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool TGAResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		return true;
	}
} // End namespace (BGE)
