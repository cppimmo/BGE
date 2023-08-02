/*=============================================================================*
 * Random.hpp - Random number generation tools.
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
#ifndef _BGE_RANDOM_HPP_
#define _BGE_RANDOM_HPP_

namespace BGE::Math
{
	// Structure to store a range for use with the Random class.
	template <Math::Numeric Type>
	struct RandomRange
	{
		Type minValue, maxValue;
	};
	// Helper function to determine if a range is valid.
	template <Math::Numeric Type>
	constexpr bool IsRandomRangeValid(const RandomRange<Type> &range);
	
	class Random
	{
		std::uint64_t m_seed;
		std::mt19937 m_engine;
	public:
		explicit Random(std::uint64_t seedNum = std::time(nullptr));
	
		template <Math::Numeric Type>
		Type Generate(const RandomRange<Type> &range) const;
		void Reseed(std::uint64_t seedNum = std::time(nullptr));
		std::uint64_t GetSeed(void) const noexcept;
	};
	
	template <Math::Numeric Type>
	inline Type Random::Generate(const RandomRange<Type> &range) const
	{
		BGE_ASSERT(IsRandomRangeValid(range)); // Check for valid range
	
		std::uniform_real_distribution<Type> distro(range.minValue, range.maxValue);
		return distro(m_engine);
	}
	
	template <Math::Numeric Type>
	inline constexpr bool IsRandomRangeValid(const RandomRange<Type> &range)
	{
		return !(range.minValue < range.maxValue);
	}
} // End namespace (BGE::Math)

#endif /* !_BGE_RANDOM_HPP_ */