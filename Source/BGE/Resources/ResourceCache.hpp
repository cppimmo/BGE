/*******************************************************************************
 * @file   ResourceCache.hpp
 * @author Brian Hoffpauir
 * @date   11.29.2024
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
#ifndef _BGE_RESOURCECACHE_HPP_
#define _BGE_RESOURCECACHE_HPP_

#include "Resources/Resource.hpp"
#include "Resources/ResourceHandle.hpp"
#include "Resources/ResourceLoader.hpp"
#include "Resources/ResourceFile.hpp"

namespace BGE
{
	class ResourceCache; // Forware declare
	BGE_DECLARE_PTR(ResourceCache);

	class ResourceCache
	{
	public:
		//! Progess callback used when preloading resources.
		using ProgressCallback = std::function<void(int, bool &)>;
	private:
		friend class ResourceHandle;

		StrongIResourceFilePtr m_pResourceFile;
		std::size_t m_cacheSize; //!< Total memory size (bytes)
		std::size_t m_allocated; //!< Total memory allocated (bytes)

		ResourceHandleList m_resourceHandles; //!< LRU resource handle list
		ResourceHandleMap m_resources;
		ResourceLoaderList m_resourceLoaders;
	public:
		/**
		 * @brief Constructs a ResourceCache with the specified size and resource file.
		 *
		 * This constructor initializes the resource cache with a maximum allowed size in bytes
		 * and a pointer to the resource file to manage cached resources.
		 *
		 * @param size The maximum size of the resource cache, in bytes.
		 * @param pResourceFile A strong pointer to the resource file used by the cache.
		 */
		ResourceCache(std::size_t size, StrongIResourceFilePtr pResourceFile);
		virtual ~ResourceCache(void);

		bool Init(void);
		void RegisterLoader(StrongIResourceLoaderPtr pLoader);
		StrongResourceHandlePtr GetHandle(const Resource &kResource);

		std::size_t Preload(std::string_view pattern, ProgressCallback progressCallback);
		std::vector<std::string> Match(std::string_view pattern);

		void Flush(void);
		bool IsUsingDevelopmentDirectories(void) const;
	protected:
		bool MakeRoom(std::size_t size);
		char *Allocate(std::size_t size);
		void Free(StrongResourceHandlePtr pResourceHandle);

		StrongResourceHandlePtr Load(const Resource &kResource);
		StrongResourceHandlePtr Find(const Resource &kResource) const;
		void Update(StrongResourceHandlePtr pResourceHandle);

		void FreeResources(std::size_t count = 1);
		//! Called when memory associated with a resource is freed.
		void MemoryHasBeenFreed(std::size_t size);
	};
} // End namespace (BGE)

#endif /* !_BGE_RESOURCECACHE_HPP_ */
