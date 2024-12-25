#ifndef _BGE_ALLOCATOR_HPP_
#define _BGE_ALLOCATOR_HPP_

namespace BGE
{
	class Allocator; // Forward declare
	BGE_DECLARE_PTR(Allocator);

	/**
	 * @brief .
	 */
	class Allocator : public INonCopyable
	{
	protected:
		std::size_t size;
	public:
		virtual ~Allocator(void) = default;
		// Interface:
		virtual void VInit(std::size_t size, void *pMemory) = 0;
		[[nodiscard]] virtual void *VAllocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;
		virtual void VFree(void *pMemory) = 0;

		template <typename Type>
		[[nodiscard]] void *AllocateType(void);
	};

	class IAllocatorFactory
	{
	public:
		virtual ~IAllocatorFactory(void) = default;
		// Interface:

	};
} // End namespace (BGE)

#endif /* !_BGE_ALLOCATOR_HPP_ */
