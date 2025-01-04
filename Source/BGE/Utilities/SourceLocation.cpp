/*******************************************************************************
 * @file   SourceLocation.cpp
 * @author Brian Hoffpauir
 * @date   01.03.2024
 * @brief  Implementation of the `SourceLocation` class, providing a wrapper
 *         around std::source_location.
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
#include "Engine/EngineStd.hpp"
#include "Utilities/SourceLocation.hpp"

#include <sstream>

namespace BGE
{
	SourceLocation::SourceLocation(std::source_location location)
		: m_location(location)
	{
	}

	std::string SourceLocation::VToString(void) const
	{
		// Format the source location in the string stream
		std::ostringstream oss;
		oss << "file: "
			<< m_location.file_name() << '('
			<< m_location.line() << ':'
			<< m_location.column() << ") `"
			<< m_location.function_name() << "`";
		return oss.str();
	}

	const std::source_location &SourceLocation::Get(void) const noexcept
	{
		return m_location;
	}
} // End namespace (BGE)
