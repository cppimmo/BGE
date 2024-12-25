#ifndef _BGE_SIMPLEALLOCATOR_HPP_
#define _BGE_SIMPLEALLOCATOR_HPP_

#include "Memory/Allocator.hpp"

namespace BGE
{
	class SimpleAllocator; // Forward declare
	BGE_DECLARE_PTR(SimpleAllocator);

	/**
	 * @brief .
	 */
	class SimpleAllocator final : public Allocator
	{
	public:
		// Allocator's interface:
		virtual void VInit(std::size_t size, void *pMemory) override;
		[[nodiscard]] virtual void *VAllocate(std::size_t size, std::size_t alignment = 0) override;
		virtual void VFree(void *pMemory) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_SIMPLEALLOCATOR_HPP_ */
