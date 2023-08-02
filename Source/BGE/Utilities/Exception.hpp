/*=============================================================================*
 * Exception.hpp - Base exception.
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
#ifndef _BGE_EXCEPTION_HPP_
#define _BGE_EXCEPTION_HPP_

namespace BGE
{
/**
 * Exception class inherited from std::exception.
 */
class Exception : public std::exception
{   // Not necessary to access from sub-classes:
	std::string_view m_filename;
	std::string_view m_functionName;
	std::size_t m_lineNum;
protected:
	mutable std::string_view m_message;
public: // Constructor & default special member functions.
	Exception(std::string_view message, std::string_view filename,
			  std::string_view functionName, std::size_t lineNum);
	Exception(const Exception &) = default;
	Exception &operator=(const Exception &) = default;
	Exception(Exception &&) noexcept = default;
	Exception &operator=(Exception &&) noexcept = default;

	virtual std::string_view VWhat(void) const noexcept; // Message
	virtual std::string_view VType(void) const noexcept; // Exception type as string
	std::string_view GetFunctionName(void) const noexcept; // Function name
	std::string_view GetFilename(void) const noexcept; // Filename
	std::size_t GetLineNum(void) const noexcept; // Line in file
protected: // Subs can override standard what() if needed.
	virtual const char *what(void) const noexcept { return m_message.data(); }
};

} // End namespace (BGE)

// __FUNCTION__ is non-standard; use __func__.  __FILE__ & __LINE__
#define BGE_THROW(MSG) \
do \
{ \
	const auto kSourceLoc = std::source_location::current(); \
	throw Exception(MSG, kSourceLoc.file_name(), kSourceLoc.function_name(), kSourceLoc.line()) \
} \
while (0) \

#define BGE_THROW_IF(COND, MSG) \
do \
{ \
	if (COND) \
	{ \
		const auto kSourceLoc = std::source_location::current(); \
		throw Exception(MSG, kSourceLoc.file_name(), kSourceLoc.function_name(), kSourceLoc.line()) \
	} \
} \
while (0) \

#endif /* !_BGE_EXCEPTION_HPP_ */