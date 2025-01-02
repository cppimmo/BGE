#ifndef _BGE_STACKALLOCATOR_HPP_
#define _BGE_STACKALLOCATOR_HPP_

namespace BGE
{
	class StackAllocator; // Forward declare
	BGE_DECLARE_PTR(StackAllocator);

	/**
	 * @brief .
	 */
#if 0
	class StackAllocator : public BaseAllocator
	{
	private:
		void* start;
		std::size_t size;
		std::size_t offset;
	public:
		StackAllocator(void* memory, std::size_t memorySize)
			: start(memory), size(memorySize), offset(0) {}

		void* Allocate(std::size_t size, std::size_t alignment) override
		{
			std::size_t alignedOffset = (offset + alignment - 1) & ~(alignment - 1);
			if (alignedOffset + size > this->size) return nullptr;

			void* ptr = static_cast<char*>(start) + alignedOffset;
			offset = alignedOffset + size;
			allocatedSize += size;
			return ptr;
		}

		void Deallocate(void* ptr) override {}
		void Reset() override { offset = 0; allocatedSize = 0; }
	};
#endif
} // End namespace (BGE)

#endif /* !_BGE_STACKALLOCATOR_HPP_ */
