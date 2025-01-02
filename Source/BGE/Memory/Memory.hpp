/*******************************************************************************
 * @file   Memory.hpp
 * @author Brian Hoffpauir
 * @date   12.09.2024
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
#ifndef _BGE_MEMORY_HPP_
#define _BGE_MEMORY_HPP_

#include <type_traits>

// Include the allocators:
#include "Memory/Allocator.hpp"
#include "Memory/Allocators/StackAllocator.hpp"
#include "Memory/Allocators/PoolAllocator.hpp"
#include "Memory/Allocators/FreeListAllocator.hpp"
#include "Memory/Allocators/LinearAllocator.hpp"
#include "Memory/Allocators/SimpleAllocator.hpp"

namespace BGE
{
	class MemoryManager; // Foward declare
	BGE_DECLARE_PTR(MemoryManager);

	// Memory size constants:
    constexpr std::uint64_t kONE_KILOBYTE = 1024;
    constexpr std::uint64_t kONE_MEGABYTE = kONE_KILOBYTE * 1024;
    constexpr std::uint64_t kONE_GIGABYTE = kONE_MEGABYTE * 1024;

    inline std::uint64_t operator""_KiB(unsigned long long value)
	{
		return value * 1024;
	}

    inline std::uint64_t operator""_MiB(unsigned long long value)
	{
		return value * 1024 * 1024;
	}

	inline std::uint64_t operator""_GiB(unsigned long long value)
	{
		return value * 1024 * 1024 * 1024;
	}

	class MemoryManager final : public INonCopyable, public INonMovable
	{
	private:
		static UniqueMemoryManagerPtr s_pInstance;

		void *m_pAllocatorPool; //!< Pool of memory for custom allocators.
		std::size_t m_poolSize;
		bool m_bInitialized = false;
	public:
		static MemoryManager &GetInstance(void) noexcept;
		bool Init(void);
		void Shutdown(void);

		template <typename Allocator, typename... Args>
		requires DerivedFromIAllocator<Allocator>
		Allocator *CreateAllocator(Args... args)
		{
			return new(m_pAllocatorPool) Allocator(args...);
		}

		void DestroyAllocator(IAllocator *pAllocator);
		MemoryManager(std::size_t poolSize);
		~MemoryManager(void);
	private:

	};
} // End namespace (BGE)

/*
 * NOTE: Overloaded new and delete operators need to check if the global application layer
 * memory manager has been initialized.  If not, it should use std::malloc & std::free, and
 * store addresses of allocated memory so the memory manager does not attempt to free memory
 * allocated with std::malloc once it has been initialized.
 */

#if defined(_DEBUG) // Only on Windows IIRC
#define BGE_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__) // Use overloaded debug new operator
#else
#define BGE_NEW new
#endif

#ifndef BGE_SAFE_DELETE
#define BGE_SAFE_DELETE(X) if (X) { delete X; X = nullptr; }
#endif

#ifndef BGE_SAFE_DELETE_ARRAY
#define BGE_SAFE_DELETE_ARRAY(X) if (X) { delete[] X; X = nullptr; }
#endif

#endif /* !_BGE_MEMORY_HPP_ */

