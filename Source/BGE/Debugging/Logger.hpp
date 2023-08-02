/*******************************************************************************
 * @file   Logger.hpp
 * @author Brian Hoffpauir
 * @date   01.08.2023
 * @brief  Logging facilities.
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
#ifndef _BGE_LOGGER_HPP_
#define _BGE_LOGGER_HPP_

#include <SDL.h>
#include <Utilities/Utils.hpp>

#include <cstddef>
#include <string_view>

//! Logging facilities namespace.
namespace BGE::Logger
{
	// Use flags to display output to console or file.
	enum DisplayFlag : std::uint8_t
	{
		kDISPLAY_FLAG_FILE = 0x01,
		kDISPLAY_FLAG_CONSOLE = 0x02
	};
	
	// Base logging levels:
	enum struct Level
	{
		Fatal = 0,
		Error,
		Warning,
		Info,
		Log
	};
		
	class ErrorMessenger
	{
		bool m_isEnabled;
		bool m_isFatal;
	public:
		explicit ErrorMessenger(bool isFatal);
		int Show(std::string_view tagName, std::string_view msgFormat, ...);
		bool Enabled(void) const noexcept;
		bool Fatal(void) const noexcept;
	};
		
	void Init(std::string_view configFilename);
	void Destroy(void);
	constexpr std::string_view LevelToString(Level level) noexcept;
	int Write(std::string_view tagName, std::string_view msgFormat, ...);
	void SetMaxMessageLength(std::size_t length);
	// https://stackoverflow.com/questions/18803940/how-to-make-enum-class-to-work-with-the-bit-or-feature
	void SetDisplayFlags(std::string_view tagName, std::uint8_t flags);
	void LogOutputFunc_SDL(void *pUserData, int category, SDL_LogPriority priority, const char *pMessage);
} // End namespace (BGE::Logger)

inline constexpr std::string_view BGE::Logger::LevelToString(Level level) noexcept
{
	using namespace std::string_view_literals;
	switch (level)
	{
	case Level::Fatal:
		return "FATAL"sv;
		break;
	case Level::Error:
		return "ERROR"sv;
		break;
	case Level::Warning:
		return "WARNING"sv;
		break;
	case Level::Info:
		return "INFO"sv;
		break;
	case Level::Log:
		return "LOG"sv;
		break;
	default:
		return ""sv;
		break;
	}
}

#if defined(BGE_CONFIG_DEBUG) || defined(BGE_CONFIG_PROFILE) // Debug mode

#define BGE_FATAL(...) \
do \
{ \
	using namespace BGE::Logger; \
	static ErrorMessenger *s_pMessenger = new ErrorMessenger(true); \
	s_pMessenger->Show(LevelToString(Level::Fatal), __VA_ARGS__);	\
} \
while (0) \

#define BGE_FATAL_IF(COND, ...)	\
do \
{ \
	if (COND) \
	{ \
		using namespace BGE::Logger; \
		static ErrorMessenger *s_pMessenger = new ErrorMessenger(true); \
		s_pMessenger->Show(LevelToString(Level::Fatal), __VA_ARGS__); \
	} \
} \
while (0) \

#define BGE_ERROR(...) \
do \
{ \
	using namespace BGE::Logger; \
	static ErrorMessenger *s_pMessenger = new ErrorMessenger(false); \
	s_pMessenger->Show(LevelToString(Level::Error), __VA_ARGS__); \
} \
while (0) \

#define BGE_ERROR_IF(COND, ...) \
do \
{ \
	if (COND) \
	{ \
		using namespace BGE::Logger; \
		static ErrorMessenger *s_pMessenger = new ErrorMessenger(false); \
		s_pMessenger->Show(LevelToString(Level::Error), __VA_ARGS__); \
	} \
} \
while (0) \

#define BGE_WARNING(...)  \
do \
{ \
	using namespace BGE::Logger; \
	Write(LevelToString(Level::Warning), __VA_ARGS__); \
} \
while (0) \

#define BGE_WARNING_IF(COND, ...)  \
do \
{ \
	if (COND) \
	{ \
		using namespace BGE::Logger; \
		Write(LevelToString(Level::Warning), __VA_ARGS__); \
	} \
} \
while (0) \

#define BGE_INFO(...)  \
do \
{ \
	using namespace BGE::Logger; \
	Write(LevelToString(Level::Info), __VA_ARGS__); \
} \
while (0) \

#define BGE_INFO_IF(COND, ...)  \
do \
{ \
	if (COND) \
	{ \
		using namespace BGE::Logger; \
		Write(LevelToString(Level::Info), __VA_ARGS__); \
	} \
} \
while (0) \

#define BGE_LOG(...)  \
do \
{ \
	using namespace BGE::Logger; \
	Write(__VA_ARGS__); \
} \
while (0) \

#define BGE_LOG_IF(COND, ...) \
do \
{ \
	if (COND) \
	{ \
		using namespace BGE::Logger; \
		Write(__VA_ARGS__); \
	} \
} \
while (0) \

#define BGE_ASSERT(EXPR) \
do \
{ \
	if (!(EXPR)) \
	{ \
		using namespace BGE::Logger; \
		static ErrorMessenger *pMessenger = new ErrorMessenger(true); \
		pMessenger->Show(#EXPR, ""); \
	} \
} \
while (0) \

#else // Release mode

#define BGE_FATAL(...) do { ; } while (0)
#define BGE_FATAL_IF(COND, ...) do { ; } while (0)
#define BGE_ERROR(...) do { ; } while (0)
#define BGE_ERROR_IF(COND, ...) do { ; } while (0)
#define BGE_WARNING(...) do { ; } while (0)
#define BGE_WARNING_IF(COND, ...) do { ; } while (0)
#define BGE_INFO(...) do { ; } while (0)
#define BGE_INFO_IF(COND, ...) do { ; } while (0)
#define BGE_LOG(...) do { ; } while (0)
#define BGE_LOG_IF(COND, ...) do { ; } while (0)
#define BGE_ASSERT(EXPR, ...) do { ; } while (0)

#endif /* def BGE_CONFIG_DEBUG || BGE_CONFIG_PROFILE */

#endif /* !_BGE_LOGGER_HPP_ */
