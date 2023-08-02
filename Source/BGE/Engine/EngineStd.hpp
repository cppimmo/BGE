/*******************************************************************************
 * @file   EngineStd.hpp
 * @author Brian Hoffpauir
 * @date   01.08.2023
 * @brief  Standard project items.
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
 ******************************************************************************/
#ifndef _BGE_ENGINESTD_HPP_
#define _BGE_ENGINESTD_HPP_

// Common library headers:
#include <SDL.h>
#include <imgui.h>
#include <implot.h>
#include <glad/glad.h>
#include <tinyxml2.h>
// Common standard headers:
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <memory>
#include <new>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <map>
#include <span>
#include <type_traits>
#include <concepts>
#include <numeric>
#include <numbers>
#include <limits>
#include <algorithm>
#include <compare>
#include <chrono>
#include <ratio>
#include <exception>
#include <source_location>
// Common project headers:
#include "Utilities/Types.hpp"
#include "Utilities/Templates.hpp"
#include "Engine/Interfaces.hpp"
#include "Utilities/Utils.hpp"
#include "Debugging/Logger.hpp"
#include "Utilities/Exception.hpp"
#include "Utilities/String.hpp"
#include "Utilities/Timer.hpp"
#include "Utilities/Math.hpp"
//#include "Utilities/Random.hpp"
#include "Engine/BGUT.hpp"
// Useful macros:

// Platform macro constants (use #if, etc.):
// Article about platform detection: https://iq.opengenus.org/detect-operating-system-in-c/
#define BGE_PLATFORM_WIN _WIN32
#define BGE_PLATFORM_WINDBG (_WIN32 && _DEBUG)
#define BGE_PLATFORM_WIN64 _WIN64

#if BGE_PLATFORM_WIN
#include "Engine/Winclude.h" // Include Windows header
#endif

#define BGE_PLATFORM_WIN32 (_WIN32 && !_WIN64)
#define BGE_PLATFORM_CRAPPLE __APPLE__ // Crapple
#define BGE_PLATFORM_MACOS TARGET_OS_MAC
#define BGE_PLATFORM_UNIX __unix__
#define BGE_PLATFORM_LINUX __linux__
#define BGE_PLATFORM_BSD BSD
#define BGE_PLATFORM_FREEBSD __FreeBSD__
// Configuration macro constants (defined during compilation):
// BGE_CONFIG_DEBUG // For debugging
// BGE_CONFIG_PROFILE // For profiling (disables certain parts of DEBUG config)
// BGE_CONFIG_RELEASE // For final versions

//! Primary engine namespace.
namespace BGE
{
	//! Interface for non-copyable class types.
	class INonCopyable
	{
	public:
		INonCopyable(void) = default;
		INonCopyable(const INonCopyable &) = delete;
		INonCopyable &operator=(const INonCopyable &) = delete;
		virtual ~INonCopyable(void) = default;
	};
	//! Interface for non-moveable class types.
	class INonMoveable
	{
	public:
		INonMoveable(void) = default;
		INonMoveable(const INonMoveable &) = delete;
		INonMoveable &operator=(const INonMoveable &) = delete;
		virtual ~INonMoveable(void) = default;
	};
} // End namespace (BGE)

#if defined(_DEBUG) // Only on Windows IIRC
#define BGE_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__) // Use overloaded debug new operator
#else
#define BGE_NEW new
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(X) if (X) { delete X; X = nullptr; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(X) if (X) { delete[] X; X = nullptr; }
#endif

#include "Engine/Engine.hpp"

namespace BGE
{
	// Engine entry point:
	extern int EngineMain(int numArgs, char *pArgv[]);
} // End namespace (BGE)

#endif /* !_BGE_ENGINESTD_HPP_ */
