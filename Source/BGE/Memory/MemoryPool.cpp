/*=============================================================================*
 * MemoryPool.cpp - For small allocations.
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
#include "Engine/EngineStd.hpp"
#include "MemoryPool.hpp"

BGE::MemoryPool::MemoryPool(void)
	: m_ppRawMemoryArray(nullptr),
	  m_pHead(nullptr),
	  m_chunkSize(0), m_numChunks(0),
	  m_memArraySize(0),
	  m_toAllowResize(false)
{
	Reset();
}

BGE::MemoryPool::~MemoryPool(void)
{
	Destroy();
}

bool BGE::MemoryPool::Init(std::size_t chunkSize, std::size_t numChunks)
{
	return false;
}

void BGE::MemoryPool::Destroy(void)
{
}

void *BGE::MemoryPool::Alloc(void)
{
	return nullptr;
}

void BGE::MemoryPool::Free(void *pMem)
{
}

std::size_t BGE::MemoryPool::GetChunkSize(void) const noexcept
{
	return m_chunkSize;
}

std::size_t BGE::MemoryPool::GetNumChunks(void) const noexcept
{
	return m_numChunks;
}

void BGE::MemoryPool::SetAllowResize(bool toAllowResize) noexcept
{
	m_toAllowResize = toAllowResize;
}

void BGE::MemoryPool::Reset(void)
{
}

bool BGE::MemoryPool::GrowMemoryArray(void)
{
	// allocate a new array
	const std::size_t kAllocationSize = sizeof(unsigned char *) * (m_memArraySize + 1);
	unsigned char **ppNewMemArray = static_cast<unsigned char **>(std::malloc(kAllocationSize));

	// determine if allocation succeeded
	if (!ppNewMemArray)
		return false; // failure
	
	// copy any existing memory pointer over
	for (std::size_t index = 0; index < m_memArraySize; ++index)
	{
		ppNewMemArray[index] = m_ppRawMemoryArray[index];
	}
	/**
	 * Allocate a new block of memory. Indexing m_memArraySize her is safe because it hasn't
	 * been incremented yet to reflect the new size.
	 */
	ppNewMemArray[m_memArraySize] = AllocateNewMemoryBlock();

	// attach the block to the end of the current memory list
	if (m_pHead) // if not NULL
	{
		auto *pCurr = m_pHead;
		auto *pNext = GetNext(m_pHead);
		while (pNext) // while not NULL
		{
			pCurr = pNext;
			pNext = GetNext(pNext);
		}
		SetNext(pCurr, ppNewMemArray[m_memArraySize]);
	}
	else
	{
		m_pHead = ppNewMemArray[m_memArraySize];
	}

	// destroy the old memory array
	if (m_ppRawMemoryArray)
		std::free(m_ppRawMemoryArray);

	// assign the new memory array and increment the size count
	m_ppRawMemoryArray = ppNewMemArray;
	++m_memArraySize;

	return true; // success
}

unsigned char *BGE::MemoryPool::AllocateNewMemoryBlock(void)
{
	return nullptr;
}

unsigned char *BGE::MemoryPool::GetNext(unsigned char *pBlock)
{
	return nullptr;
}

void BGE::MemoryPool::SetNext(unsigned char *pBlockToChange, unsigned char *pRawNext)
{
}
