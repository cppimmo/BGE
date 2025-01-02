#include "Engine/EngineStd.hpp"
#include "Memory/Allocators/LinearAllocator.hpp"

namespace BGE
{
	LinearAllocator::LinearAllocator(std::size_t size, void *pStart) noexcept
		: BaseAllocator(size), m_pCurrent(pStart)
	{
	}

	LinearAllocator::LinearAllocator(LinearAllocator &&other) noexcept
		: BaseAllocator(std::move(other)), m_pCurrent(other.m_pCurrent)
	{
		other.m_pCurrent = nullptr;
	}

	LinearAllocator &LinearAllocator::operator=(LinearAllocator &&rhs) noexcept
	{
		BaseAllocator::operator=(std::move(rhs));
		m_pCurrent = rhs.m_pCurrent;
		rhs.m_pCurrent = nullptr;
		return *this;
	}

	LinearAllocator::~LinearAllocator(void)
	{
		VReset(); // Reset the allocator state on destruction
	}

	void *LinearAllocator::VAllocate(std::size_t size, std::size_t alignment)
	{
		BGE_ASSERT(size > 0 && alignment > 0);

		std::size_t adjustment = AlignForwardAdjustment(m_pCurrent, alignment);
		if (m_usedSize + adjustment + size > m_totalSize)
		{
			throw std::bad_alloc();
		}

		void *pAlignedAddr = PointerAdd(m_pCurrent, adjustment);
		m_pCurrent = PointerAdd(pAlignedAddr, size);

		m_usedSize += adjustment + size;
		++m_allocCount;
		return pAlignedAddr;
	}

	void LinearAllocator::VDeallocate(void *pMemory)
	{
		// Linear allocator does not support deallocation of individual allocations
	}

	void LinearAllocator::VReset(void)
	{
		//m_pCurrent = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(m_pStart));
		m_usedSize = 0;
		m_allocCount = 0;
	}

	void *LinearAllocator::GetCurrent(void) const noexcept
	{
		return m_pCurrent;
	}
} // End namespace (BGE)
