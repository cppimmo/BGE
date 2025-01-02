#ifndef _BGE_ALLOCATOR_HPP_
#define _BGE_ALLOCATOR_HPP_

namespace BGE
{
	class IAllocator; // Forward declare
	BGE_DECLARE_PTR(IAllocator);
	class BaseAllocator; // Forward declare
	BGE_DECLARE_PTR(BaseAllocator);

	//! Concept to ensure Type is derived from IAllocator.
	template <typename Type>
	concept DerivedFromIAllocator = std::is_base_of_v<IAllocator, Type>;

	std::size_t AlignForwardAdjustment(const void *const kpkMemory, std::size_t alignment) noexcept;
	void *PointerAdd(const void *const kpkMemory, std::uintptr_t amount) noexcept;

	/**
	 * @brief .
	 */
	class IAllocator
	{
	public:
		//! Default constructor.
		IAllocator(void) = default;

		//! Default move constructor.
		IAllocator(IAllocator &&) noexcept = default;

		//! Default move assignment operator.
		IAllocator &operator=(IAllocator &&) noexcept = default;

		//! Default destructor.
		virtual ~IAllocator(void) = default;

		// Interface:
		[[nodiscard]] virtual void *VAllocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;
		virtual void VDeallocate(void *pMemory) = 0;

		virtual std::size_t VGetTotalSize(void) const = 0;
		virtual std::size_t VGetUsedSize(void) const = 0;
		virtual std::size_t VGetAllocCount(void) const = 0;
		virtual void VReset(void) = 0; // Optional for stack and linear allocators

		//template <typename Type>
		//[[nodiscard]] void *AllocateType(void);
	};

	/**
	 * @brief .
	 */
	class BaseAllocator : public IAllocator
	{
	protected:
		std::size_t m_totalSize; //!< The size allocated by this allocator.
		std::size_t m_usedSize; //!< The size currently being used by this allocator.
		std::size_t m_allocCount; //!< The number of allocations made by this allocator.
	public:
		explicit BaseAllocator(std::size_t totalSize) noexcept
			: m_totalSize(totalSize), m_usedSize(0), m_allocCount(0)
		{
		}

		//! Default move constructor.
		BaseAllocator(BaseAllocator &&) noexcept = default;

		//! Default move assignment operator.
		BaseAllocator &operator=(BaseAllocator &&) noexcept = default;

		//! Default destructor.
		virtual ~BaseAllocator(void) override = default;

		virtual std::size_t VGetTotalSize(void) const override { return m_totalSize; }
		virtual std::size_t VGetUsedSize(void) const override { return m_usedSize; }
		virtual std::size_t VGetAllocCount(void) const override { return m_allocCount; }
	};

	inline std::size_t AlignForwardAdjustment(const void *const kpkMemory, std::size_t alignment) noexcept
	{
		const auto kAddress = reinterpret_cast<std::uintptr_t>(kpkMemory);
		const auto kAligned = (kAddress - 1u + alignment) & -alignment;
		return kAligned - kAddress;
	}

	inline void *PointerAdd(const void *const kpkMemory, std::uintptr_t amount) noexcept
	{
		return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(kpkMemory) + amount);
	}
} // End namespace (BGE)

#endif /* !_BGE_ALLOCATOR_HPP_ */
