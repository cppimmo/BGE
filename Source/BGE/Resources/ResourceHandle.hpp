/*******************************************************************************
 * @file   ResourceHandle.hpp
 * @author Brian Hoffpauir
 * @date   11.29.2024
 * @brief  Handle to resources in the cache.
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
#ifndef _BGE_RESOURCEHANDLE_HPP_
#define _BGE_RESOURCEHANDLE_HPP_

#include "Resources/Resource.hpp"
//#include "Resources/ResourceCache.hpp"

namespace BGE
{
	class Resource; // Forward declare
	class ResourceCache; // Forward declare
	class ResourceHandle; // Forward declare
	BGE_DECLARE_PTR(ResourceHandle);

	//! Scoped enum with all possible resource types.
	enum struct ResourceType
	{
		kXML,      /**< .xml files. */
		kJSON,     /**< .json files. */
		kScript,   /**< .lua files. */
		kOGG,      /**< .ogg files. */
		kWAV,      /**< .wav files. */
		kGLSL,     /**< .glsl files. */
		kSPIRV,    /**< .spv files. */
		kJPEG,     /**< .jpeg files. */
		kPNG,      /**< .png files. */
		kBMP,      /**< .bmp files. */
		kTGA,      /**< .tga files. */
		kUnknown   /**< Files of unknown type. */
	};

	//! Least recently used cache list.
	using ResourceHandleList = std::list<StrongResourceHandlePtr>;
	//! Maps identifiers to resource data (handles).
	using ResourceHandleMap = std::unordered_map<std::string, StrongResourceHandlePtr>;

	class ResourceHandle
	{
		friend class ResourceCache;
	protected:
		Resource m_resource;
		ResourceType m_type = ResourceType::kUnknown;
		char *m_pBuffer;
		std::size_t m_size;
		StrongIResourceExtraDataPtr m_pExtraData;
		ResourceCache *m_pResourceCache;
	public:
		ResourceHandle(const Resource &resource, char *pBuffer, std::size_t size, ResourceCache *pResourceCache);
		virtual ~ResourceHandle(void);

		// Accessors:
		const std::string &GetName(void) const noexcept;
		ResourceType GetType(void) const noexcept;
		std::size_t GetSize(void) const noexcept;
		char *Buffer(void) const;
		char *WritableBuffer(void);
		StrongIResourceExtraDataPtr GetExtraData(void);
		// Modifiers:
		void SetType(ResourceType type) noexcept;
		void SetExtraData(StrongIResourceExtraDataPtr pExtraData);
	};
} // End namespace (BGE)

#endif /* _BGE_RESOURCEHANDLE_HPP_ */
