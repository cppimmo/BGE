#include "Engine/EngineStd.hpp"
#include "Memory/SimpleAllocator.hpp"

// Allocator's interface:
void BGE::SimpleAllocator::VInit(std::size_t size, void *pMemory)
{
}

void *BGE::SimpleAllocator::VAllocate(std::size_t size, std::size_t alignment)
{
	void *pMemory = std::malloc(size);
	return pMemory;
}

void BGE::SimpleAllocator::VFree(void *pMemory)
{
	BGE_ASSERT(pMemory);
	std::free(pMemory);
}
