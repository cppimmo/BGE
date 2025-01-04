/*******************************************************************************
 * @file   SourceLocation.hpp
 * @author Brian Hoffpauir
 * @date   01.03.2024
 * @brief  Provides a wrapper around std::source_location for enhanced
 *         usability.
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
#ifndef _BGE_SOURCELOCATION_HPP_
#define _BGE_SOURCELOCATION_HPP_

#include <string>
#include <source_location>

namespace BGE
{
	class SourceLocation; // Forward declare
	BGE_DECLARE_PTR(SourceLocation);

	/**
	 * @brief Wrapper around std::source_location.
	 */
	class SourceLocation final : public IStringable
	{
	private:
		std::source_location m_location; //!< The contained source location.
	public:
		/**
		 * @brief Constructs a `SourceLocation` object with a specified or default location.
		 *
		 * @param location The `std::source_location` to encapsulate (default is the current source location).
		 */
		explicit SourceLocation(std::source_location location = std::source_location::current());

		// Default special member functions:
		SourceLocation(const SourceLocation &) = default;
		SourceLocation(SourceLocation &&) noexcept = default;
		SourceLocation &operator=(const SourceLocation &) = default;
		SourceLocation &operator=(SourceLocation &&) noexcept = default;

		// IStringable's interface:
		/**
		 * @brief Converts the `SourceLocation` into a human-readable string.
		 *
		 * Implements the `IStringable` interface.
		 * @return A string representing the source file, line, and optionally the function name.
		 */
		virtual std::string VToString(void) const override;

		// Accessors:
		/**
		 * @brief Retrieves the encapsulated `std::source_location`.
		 *
		 * @return A constant reference to the contained `std::source_location`.
		 */
		const std::source_location &Get(void) const noexcept;
	};
} // End namespace (BGE)

#endif /* !_BGE_SOURCELOCATION_HPP_ */
