/*******************************************************************************
 * @file   Exception.hpp
 * @author Brian Hoffpauir
 * @date   01.03.2024
 * @brief  Base exception.
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
#ifndef _BGE_EXCEPTION_HPP_
#define _BGE_EXCEPTION_HPP_

#include "Utilities/SourceLocation.hpp"

namespace BGE
{
	/**
	* @brief Exception class inherited from std::exception.
	*/
	class Exception : public std::exception
	{   // Not necessary to access from sub-classes:
	protected:
		mutable std::string m_message;
	private:
		SourceLocation m_location;
	public: // Constructor & default special member functions.
		explicit Exception(std::string_view message);
		Exception(std::string_view message, SourceLocation location);
		Exception(const Exception &) = default;
		Exception(Exception &&) noexcept = default;
		Exception &operator=(const Exception &) = default;
		Exception &operator=(Exception &&) noexcept = default;

		virtual std::string_view VWhat(void) const noexcept; // Message
		virtual std::string_view VType(void) const noexcept; // Exception type as string
		const SourceLocation &GetLocation(void) const noexcept;
	protected: // Subs can override standard what() if needed.
		virtual const char *what(void) const noexcept { return m_message.data(); }
	};
} // End namespace (BGE)

// __FUNCTION__ is non-standard; use __func__.  __FILE__ & __LINE__
#define BGE_THROW(MSG) \
do \
{ \
	const BGE::SourceLocation kSourceLoc; \
	throw BGE::Exception(MSG, kSourceLoc); \
} \
while (0) \

#define BGE_THROW_IF(COND, MSG) \
do \
{ \
	if (COND) \
	{ \
		const BGE::SourceLocation kSourceLoc; \
		throw BGE::Exception(MSG, kSourceLoc); \
	} \
} \
while (0) \

#endif /* !_BGE_EXCEPTION_HPP_ */
