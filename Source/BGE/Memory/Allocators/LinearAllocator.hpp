#ifndef _BGE_LINEARALLOCATOR_HPP_
#define _BGE_LINEARALLOCATOR_HPP_

#include "Memory/Allocator.hpp"

namespace BGE
{
	class LinearAllocator; // Forward declare
	BGE_DECLARE_PTR(LinearAllocator);

	/**
	 * @brief .
	 */
	class LinearAllocator : public BaseAllocator
	{
	private:
		void *m_pCurrent;
	public:
		explicit LinearAllocator(std::size_t size, void *pStart) noexcept;
		LinearAllocator(LinearAllocator &&other) noexcept;
		LinearAllocator &operator=(LinearAllocator &&rhs) noexcept;
		virtual ~LinearAllocator(void) override;

		[[nodiscard]] virtual void *VAllocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) override;
		virtual void VDeallocate(void *pMemory) override;
		virtual void VReset(void) override;

		void *GetCurrent(void) const noexcept;
	};
} // End namespace (BGE)

#endif /* !_BGE_LINEARALLOCATOR_HPP_ */

