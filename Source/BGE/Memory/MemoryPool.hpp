/*=============================================================================*
 * MemoryPool.hpp - For small allocations.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
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
 *============================================================================*/
#ifndef _BGE_MEMORYPOOL_HPP_
#define _BGE_MEMORYPOOL_HPP_

namespace BGE
{
	/**
	 *
	 */
	class MemoryPool
	{
		unsigned char **m_ppRawMemoryArray; // Array of memory blocks, each split into chunks
		unsigned char *m_pHead; // Front of the memory chunk linked list
		std::size_t m_chunkSize, m_numChunks; // Size of each chunk & number of chunks per array
		std::size_t m_memArraySize; // Number of elements in the memory array
		bool m_toAllowResize; // True if the memory pool is resized when it fills
	public:
		MemoryPool(void);
		MemoryPool(const MemoryPool &) = delete; // No copy/move constructor/ops
		MemoryPool &operator=(const MemoryPool &) = delete;
		MemoryPool(MemoryPool &&) noexcept = delete;
		MemoryPool &operator=(MemoryPool &&) noexcept = delete;
		~MemoryPool(void);
	
		bool Init(std::size_t chunkSize, std::size_t numChunks);
		void Destroy(void);
		// Allocation functions:
		void *Alloc(void);
		void Free(void *pMem);
		std::size_t GetChunkSize(void) const noexcept;
		std::size_t GetNumChunks(void) const noexcept;
		// Setters:
		void SetAllowResize(bool toAllowResize) noexcept;
	private:
		// Reset internal variables:
		void Reset(void);
		// Internal memory allocation helpers:
		bool GrowMemoryArray(void);
		unsigned char *AllocateNewMemoryBlock(void);
		// Internal linked list management:
		unsigned char *GetNext(unsigned char *pBlock);
		void SetNext(unsigned char *pBlockToChange, unsigned char *pRawNext);
	};
} // End namespace (BGE)

#endif /* !_BGE_MEMORYPOOL_HPP_ */
