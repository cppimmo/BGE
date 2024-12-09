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

namespace BGE
{
	class MemoryManager; // Foward declare
	BGE_DECLARE_PTR(MemoryManager);

	// Memory size constants:
    constexpr std::uint64_t k1_KiB = 1024;
    constexpr std::uint64_t k1_MiB = k1_KiB * 1024;
    constexpr std::uint64_t k1_GiB = k1_MiB * 1024;

    inline std::uint64_t operator""_KiB(unsigned long long value)
	{
		return value * 1024;
	}

    inline std::uint64_t operator""_MiB(unsigned long long value)
	{
		return value * 1024 * 1024;

	}

	class MemoryManager
	{
		static UniqueMemoryManagerPtr s_pInstance;
	public:

	public:
		static MemoryManager &Get(void);
	private:
		MemoryManager(void); // Hide constructor to prevent instances from being made
	};
} // End namespace (BGE)

#endif /* !_BGE_MEMORY_HPP_ */

