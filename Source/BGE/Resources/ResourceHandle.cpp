/*******************************************************************************
 * @file   ResourceHandle.cpp
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
#include "Engine/EngineStd.hpp"
#include "Resources/ResourceHandle.hpp"

#include "Resources/ResourceCache.hpp"

namespace BGE
{
	ResourceHandle::ResourceHandle(const Resource &resource, char *pBuffer, std::size_t size,
								   ResourceCache *pResourceCache)
		: m_resource(resource), m_pBuffer(pBuffer), m_size(size), m_pExtraData{}, m_pResourceCache(pResourceCache)
	{
	}

	ResourceHandle::~ResourceHandle(void)
	{
		BGE_SAFE_DELETE_ARRAY(m_pBuffer);
		// Signal to resource cache that memory has been freed
		m_pResourceCache->MemoryHasBeenFreed(m_size);
	}

	const std::string &ResourceHandle::GetName(void) const noexcept
	{
		return m_resource.GetName();
	}

	ResourceType ResourceHandle::GetType(void) const noexcept
	{
		return m_type;
	}

	std::size_t ResourceHandle::GetSize(void) const noexcept
	{
		return m_size;
	}

	char *ResourceHandle::Buffer(void) const
	{
		return m_pBuffer;
	}

	char *ResourceHandle::WritableBuffer(void)
	{
		return m_pBuffer;
	}

	StrongIResourceExtraDataPtr ResourceHandle::GetExtraData(void)
	{
		return m_pExtraData;
	}

	void ResourceHandle::SetType(ResourceType type) noexcept
	{
		m_type = type;
	}

	void ResourceHandle::SetExtraData(StrongIResourceExtraDataPtr pExtraData)
	{
		m_pExtraData = pExtraData;
	}
} // End namespace (BGE)
