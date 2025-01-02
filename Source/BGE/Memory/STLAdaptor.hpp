#ifndef _BGE_STLADAPTOR_HPP_
#define _BGE_STLADAPTOR_HPP_

#include "Memory/Allocator.hpp"

namespace BGE
{
	/**
	 * @brief .
	 */
	template <typename Type, DerivedFromIAllocator Allocator>
	class STLAdaptor
	{
	public:
		using value_type = Type;
	private:
		Allocator &m_allocator;
	public:
		STLAdaptor(void) = delete;
		STLAdaptor(Allocator &allocator) noexcept
			: m_allocator(allocator)
		{
		}

		template<typename U>
		STLAdaptor(const STLAdaptor<U, Allocator> &kOther) noexcept
			: m_allocator(kOther.m_allocator)
		{
		}

		[[nodiscard]] constexpr Type* allocate(std::size_t size)
		{
			return reinterpret_cast<Type *>(m_allocator.Allocate(size * sizeof(Type), alignof(Type)));
		}

		constexpr void deallocate(Type *pMemory, [[maybe_unused]] std::size_t size) noexcept
		{
			m_allocator.VDeallocate(pMemory);
		}

		/*bool operator==(const STLAdaptor<Type, Allocator> &kRhs) const noexcept
		{
			if constexpr(std::is_base_of_v<FixedAllocator,Alloc>)
			{
				return m_allocator.GetStart() == kRhs.m_allocator.GetStart();
			}
			else
			{
				DynamicAllocator::BlockDesc *a = reinterpret_cast<DynamicAllocator *>(&m_allocator)->m_currentBlock;

				while (a->prevBlock != nullptr)
				{
					a = a->prevBlock;
				}

				DynamicAllocator::BlockDesc* b = reinterpret_cast<DynamicAllocator *>(&kRhs.m_allocator)->m_currentBlock;

				while (b->prevBlock != nullptr)
				{
					b = b->prevBlock;
				}

				return a == b;
			}
		}*/

		bool operator!=(const STLAdaptor<Type, Allocator> &kRhs) const noexcept
		{
			return !(*this == kRhs);
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_STLADAPTOR_HPP_ */
