#include "Engine/EngineStd.hpp"
#include "Resources/ResourceCache.hpp"

BGE::ResourceCache::ResourceCache(std::size_t size, StrongIResourceFilePtr pResourceFile)
	: m_pResourceFile(pResourceFile), m_cacheSize(size), m_allocated(0)
{
}

BGE::ResourceCache::~ResourceCache(void)
{
	constexpr std::size_t kFREE_COUNT = 1;
	// Free one resource at a time
	while (!m_resourceHandles.empty())
	{
		FreeResources(kFREE_COUNT);
	}
}

bool BGE::ResourceCache::Init(void)
{
	bool bResult = false;
	if (m_pResourceFile->VOpen())
	{
		BGE_LOG("Resources", "Resource cache opened successfully");
		// All caches need a default resource loader
		RegisterLoader(std::make_shared<DefaultResourceLoader>());
		bResult = true;
	}
	return bResult;
}

void BGE::ResourceCache::RegisterLoader(StrongIResourceLoaderPtr pLoader)
{
	m_resourceLoaders.push_front(pLoader);
}

BGE::StrongResourceHandlePtr BGE::ResourceCache::GetHandle(const Resource& kResource)
{
	auto pResourceHandle = Find(kResource);
	if (!pResourceHandle)
	{
		BGE_LOG("Resources", "Calling ResourceCache::Load(\"%s\")", kResource.GetName().c_str());
		pResourceHandle = Load(kResource);
		BGE_ASSERT(pResourceHandle);
	}
	else
	{
		BGE_LOG("Resources", "Calling ResourceCache::Update(\"%s\")", kResource.GetName().c_str());
		Update(pResourceHandle);
	}
	return pResourceHandle;
}

std::size_t BGE::ResourceCache::Preload(std::string_view pattern, ProgressCallback progressCallback)
{
	if (!m_pResourceFile)
	{
		return 0;
	}

	const std::size_t kNumFiles = m_pResourceFile->VGetNumResources();
	std::size_t loaded = 0;
	bool bCancel = false;
	for (std::size_t i = 0; i < kNumFiles; ++i)
	{
		Resource resource(m_pResourceFile->VGetResourceName(i));

		if (WildcardMatch(pattern, resource.GetName()))
		{
			auto pHandle = GetHandle(resource);
			++loaded;
		}

		if (progressCallback)
		{
			progressCallback(i * 100 / kNumFiles, bCancel);
		}
	}
	return loaded;
}

std::vector<std::string> BGE::ResourceCache::Match(std::string_view pattern)
{
	std::vector<std::string> matchingNames;
	if (!m_pResourceFile)
	{
		return matchingNames;
	}

	const std::size_t kNumFiles = m_pResourceFile->VGetNumResources();
	for (std::size_t i = 0; i < kNumFiles; ++i)
	{
		std::string name = StringToLower(m_pResourceFile->VGetResourceName(i));
		if (WildcardMatch(pattern, name))
		{
			matchingNames.emplace_back(name);
		}
	}
	return matchingNames;
}

void BGE::ResourceCache::Flush(void)
{
	// Free all resources from the cache
	while (!m_resourceHandles.empty())
	{
		auto pHandle = *(m_resourceHandles.begin());
		Free(pHandle);
		m_resourceHandles.pop_front();
	}
}

bool BGE::ResourceCache::IsUsingDevelopmentDirectories(void) const
{
	BGE_ASSERT(m_pResourceFile);
	return m_pResourceFile->VIsUsingDevelopmentDirectories();
}

bool BGE::ResourceCache::MakeRoom(std::size_t size)
{
	// Ensure the requested size is not larger than the total cache size
	if (size > m_cacheSize)
	{
		return false;
	}

	// Check if there's enough free memory already
	if (m_cacheSize >= size + m_allocated)
	{
		return true;
	}

	/*
	 * Return null if there's no way to alloc the memory. The previous if statement prevents
	 * arithmetic overflow for the condition in the following while loop.
	 */
	while (size > (m_cacheSize - m_allocated))
	{
		// The cache is empty, & there's still not enough room, so fail
		if (m_resourceHandles.empty())
			return false;

		// Free one resource at a time to attempt to make room
		constexpr std::size_t kFREE_COUNT = 1;
		FreeResources(kFREE_COUNT);
	}
	return true;
}

char *BGE::ResourceCache::Allocate(std::size_t size)
{
	// TODO: Use a custom allocator.
	if (!MakeRoom(size))
	{
		return nullptr;
	}

	char *pMem = BGE_NEW char[size];
	if (pMem)
	{
		m_allocated += size;
	}
	return pMem;
}

void BGE::ResourceCache::Free(StrongResourceHandlePtr pResourceHandle)
{
	m_resourceHandles.remove(pResourceHandle);
	m_resources.erase(pResourceHandle->GetName());
	/*
	 * NOTE: The resource may still be used by some subsystem, thus the cache can't count the memory as freed until the resource handle is destroyed.
	 */
	//m_allocated -= pResourceHandle->GetSize();
	//BGE_SAFE_DELETE(pResourceHandle);
}

BGE::StrongResourceHandlePtr BGE::ResourceCache::Load(const Resource& kResource)
{
	// Create a new resource & add it to the LRU list & map
	StrongIResourceLoaderPtr pResourceLoader;
	StrongResourceHandlePtr pResourceHandle;

	for (auto &pLoader : m_resourceLoaders)
	{
		if (WildcardMatch(pLoader->VGetPattern(), kResource.GetName()))
		{
			pResourceLoader = pLoader;
			break;
		}
	}

	if (!pResourceLoader)
	{
		BGE_ERROR("Default resource loader not found!");
		return nullptr;
	}

	const std::size_t kRawSize = m_pResourceFile->VGetRawResourceSize(kResource);
	// TODO: Change return type to indicate errors.
	if (kRawSize <= 0)
	{
		BGE_ERROR("Resource size returned 0 - Resource not found");
		return nullptr;
	}

	const auto kAllocSize = kRawSize + (pResourceLoader->VAddNullZero() ? 1 : 0);
	char *pRawBuffer = pResourceLoader->VUseRawFile() ? Allocate(kAllocSize) : BGE_NEW char[kAllocSize];
	std::memset(pRawBuffer, 0, kAllocSize);

	if (!pRawBuffer || m_pResourceFile->VGetRawResource(kResource, pRawBuffer) == 0)
	{
		// Resource cache out of memory
		return nullptr;
	}

	char *pBuffer = nullptr;
	std::size_t size = 0;

	if (pResourceLoader->VUseRawFile())
	{
		pBuffer = pRawBuffer;
		pResourceHandle = std::make_shared<ResourceHandle>(kResource, pBuffer, kRawSize, this);
	}
	else
	{
		size = pResourceLoader->VGetLoadedResourceSize(pRawBuffer, kRawSize);
		pBuffer = Allocate(size);
		if (!pRawBuffer || !pBuffer)
		{
			// Resource cache out of memory
			return nullptr;
		}
		pResourceHandle = std::make_shared<ResourceHandle>(kResource, pBuffer, size, this);
		bool bSuccess = pResourceLoader->VLoadResource(pRawBuffer, kRawSize, pResourceHandle);
		/*
		 * NOTE: Used for resources that are converted to a usable format upon loading (i.e.
		 * compressed files). Release the raw buffer for the resource file if it isn't needed.
		 */
		if (pResourceLoader->VDiscardRawBufferAfterLoad())
		{
			BGE_SAFE_DELETE_ARRAY(pRawBuffer);
		}

		if (!bSuccess)
		{
			// Resource cache out of memory
			return nullptr;
		}
	}

	if (pResourceHandle)
	{
		m_resourceHandles.push_front(pResourceHandle);
		m_resources[kResource.GetName()] = pResourceHandle;
	}

	BGE_ASSERT(pResourceHandle && "Default resource loader not found");
	return pResourceHandle; // Resource cache is out of memory!
}

BGE::StrongResourceHandlePtr BGE::ResourceCache::Find(const Resource& kResource) const
{
	auto findIt = m_resources.find(kResource.GetName());
	// Return nullopt for missing resource
	if (findIt == m_resources.end())
	{
		return nullptr;
	}
	return findIt->second; // Resource was found in the caches
}

void BGE::ResourceCache::Update(StrongResourceHandlePtr pResourceHandle)
{
	// Cycle the given resource handle in the LRU list
	m_resourceHandles.remove(pResourceHandle);
	m_resourceHandles.push_front(pResourceHandle);
}

void BGE::ResourceCache::FreeResources(std::size_t count)
{
	// Validate count
	if (count == 0 || m_resourceHandles.empty())
	{
		return; // Return, since there is nothing to free
	}

	// Free up to `count` resources, or stop when LRU list is empty
	std::size_t removedCount = 0;
	while (removedCount < count && !m_resourceHandles.empty())
	{
		auto gonnerIt = std::prev(m_resourceHandles.end()); // Get iterator to last element
		auto pHandle = *gonnerIt; // Deref to get strong reshandle pointer

		m_resourceHandles.pop_back(); // Remove from LRU
		m_resources.erase(pHandle->GetName()); // Remove from resource map
		++removedCount;
	}
	/*
	 * NOTE: The resource cache size can't be changed yet, because the resources memory
	 * could still be used by some subsystem retaining the ResourceHandle. The memory will
	 * only be freed once that handle is destroyed (goes out of scope).
	 */
}

void BGE::ResourceCache::MemoryHasBeenFreed(std::size_t size)
{
	m_allocated -= size;
}
