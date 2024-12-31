/*******************************************************************************
 * @file   Utils.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Various utilities.
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
#ifndef _BGE_UTILS_HPP_
#define _BGE_UTILS_HPP_

namespace BGE
{
	// C++20 concept representing an enum type.
	template <typename Type>
	concept Enum = std::is_enum<Type>::value;

	/**
	 * @brief Use to get the current system time in a string representation.
	 * @param useUnderscores Makes the string suitable for filenames.
	 * @return The current system time in a string representation.
	 */
	std::optional<std::string> GetSystemTimeString(bool useUnderscores = false);

	/**
	 * @brief Generate a UUID.
	 */
	std::string GenerateUUID(void);

	//! Cast an enum to its underyling type.
	template <Enum Type>
	inline constexpr auto ToUnderlying(Type tEnum)
	{
		return static_cast<std::underlying_type_t<Type>>(tEnum);
	}

	//! Fetch a string representation of a boolean value.
	inline constexpr std::string_view BoolToString(bool value)
	{
		return (value) ? "true" : "false";
	}

	//! Grab the strong shared pointer of a weak pointer.
	template <typename Type>
	inline std::shared_ptr<Type> MakeStrongPtr(std::weak_ptr<Type> pWeak)
	{
		if (!pWeak.expired())
			return std::shared_ptr<Type>(pWeak);
		else
			return std::shared_ptr<Type>();
	}

	//! Convert the contents of a character buffer to a 32-bit integer.
	std::int32_t ConvertBufToInt(char *pBuffer, std::size_t length);
} // End namespace (BGE::Utils)

#endif /* !_BGE_UTILS_HPP_ */
