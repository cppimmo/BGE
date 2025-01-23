/*******************************************************************************
 * @file   Logger.cpp
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
#include "Engine/EngineStd.hpp"
#include "Logger.hpp"

#include "Utilities/Utils.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <unordered_map>
#include <list>

using namespace BGE;

namespace BGE
{
    // Singleton
	class LogManager;
	BGE_DECLARE_PTR(LogManager);
	
	static LogManager *s_pLogManager = nullptr;
	inline constexpr int kNONE_WRITTEN = 0;

	struct LogMessage
	{
		std::string message;
		int repeatCount;
	};

	class LogManager : public INonCopyable, public INonMovable
	{
	public:
		enum struct ErrorDialogResult
		{
			kAbort,
			kRetry,
			kIgnore
		};
		using TagMap = std::map<std::string, std::uint8_t>;
		using ErrorMessengerList = std::list<Logger::ErrorMessenger *>;
		//! tagName -> LogMessage (stores previous message)
		using LastMessageMap = std::unordered_map<std::string, LogMessage>;
	private:
		TagMap m_tags;
		ErrorMessengerList m_errorMessengers;
		LastMessageMap m_lastMessages; 
		// Thread mutexes
	public:
		LogManager(void);
		~LogManager(void);
		
		bool Init(std::string_view configFileName);
		int Write(std::string_view tagName, std::string_view msgFormat, va_list args);
		void SetDisplayFlags(std::string_view tagName, std::uint8_t flags);
		void AddErrorMessenger(Logger::ErrorMessenger *pMessenger);
		ErrorDialogResult Error(Logger::ErrorMessenger &pMessenger, std::string_view tagName, std::string_view msgFormat...);
	private:
	};
} // End namespace (BGE)

Logger::ErrorMessenger::ErrorMessenger(bool bFatal)
	: m_bEnabled(true),
	  m_bFatal(bFatal)
{
	::s_pLogManager->AddErrorMessenger(this);
}

int Logger::ErrorMessenger::Show(std::string_view tagName, std::string_view msgFormat, ...)
{
	if (m_bEnabled)
	{
		va_list pArgList;
		va_start(pArgList, msgFormat);
		::s_pLogManager->Error(*this, tagName, msgFormat, pArgList);
		va_end(pArgList);
	}
	return 0;
}

bool Logger::ErrorMessenger::Enabled(void) const noexcept
{
	return m_bEnabled;
}

bool Logger::ErrorMessenger::Fatal(void) const noexcept
{
	return m_bFatal;
}

void Logger::Init(std::string_view configFileName)
{
	if (!::s_pLogManager)
	{
		::s_pLogManager = BGE_NEW LogManager;
		(void)::s_pLogManager->Init(configFileName);
	}
}

void Logger::Destroy(void)
{
	BGE_SAFE_DELETE(::s_pLogManager);
}

bool Logger::IsActive(void) noexcept
{
	return ::s_pLogManager != nullptr;
}

int Logger::Write(std::string_view tagName, std::string_view msgFormat, ...)
{
	if (!Logger::IsActive())
	{
		return kNONE_WRITTEN;
	}

	va_list argList;
	va_start(argList, msgFormat);
	const int kWritten = s_pLogManager->Write(tagName, msgFormat, argList);
	va_end(argList);
	
	return kWritten;
}

void Logger::SetMaxMessageLength(std::size_t length)
{
}

void Logger::SetDisplayFlags(std::string_view tagName, std::uint8_t flags)
{
	BGE_ASSERT(::s_pLogManager);
	::s_pLogManager->SetDisplayFlags(tagName, flags);
}

void Logger::LogOutputFunc_SDL(void *const pUserData, int category, SDL_LogPriority priority, const char *pMessage)
{
	using namespace std::literals::string_view_literals;
	std::string_view categoryName = "";
	switch (category)
	{
	case SDL_LOG_CATEGORY_APPLICATION:
		categoryName = "APP"sv;
		break;
	case SDL_LOG_CATEGORY_ERROR:
		categoryName = "ERROR"sv;
		break;
	case SDL_LOG_CATEGORY_ASSERT:
		categoryName = "ASSERT"sv;
		break;
	case SDL_LOG_CATEGORY_SYSTEM:
		categoryName = "SYS"sv;
		break;
	case SDL_LOG_CATEGORY_AUDIO:
		categoryName = "AUDIO"sv;
		break;
	case SDL_LOG_CATEGORY_VIDEO:
		categoryName = "VIDEO"sv;
		break;
	case SDL_LOG_CATEGORY_RENDER:
		categoryName = "RENDER"sv;
		break;
	case SDL_LOG_CATEGORY_INPUT:
		categoryName = "INPUT"sv;
		break;
	case SDL_LOG_CATEGORY_TEST:
		categoryName = "TEST"sv;
		break;
	}

	switch (priority)
	{
	case SDL_LOG_PRIORITY_VERBOSE:
	case SDL_LOG_PRIORITY_INFO:
	case SDL_LOG_PRIORITY_DEBUG:
		BGE_INFO("SDL(%s|%s)", categoryName.data(), pMessage);
		break;
	case SDL_LOG_PRIORITY_WARN:
		BGE_WARNING("SDL(%s|%s)", categoryName.data(), pMessage);
		break;
	case SDL_LOG_PRIORITY_ERROR:
	case SDL_LOG_PRIORITY_CRITICAL:
		BGE_ERROR("SDL(%s|%s)", categoryName.data(), pMessage);
		break;
	default:
		break;
	}
}

LogManager::LogManager(void)
{
	using namespace Logger;
	// Set default flags for log tags
	SetDisplayFlags(LevelToString(Level::Error), kDISPLAY_FLAG_CONSOLE | kDISPLAY_FLAG_FILE);
	SetDisplayFlags(LevelToString(Level::Warning), kDISPLAY_FLAG_CONSOLE | kDISPLAY_FLAG_FILE);
	SetDisplayFlags(LevelToString(Level::Info), kDISPLAY_FLAG_CONSOLE | kDISPLAY_FLAG_FILE);
}

LogManager::~LogManager(void)
{
	using Logger::ErrorMessenger;
	// TODO: Implement thread safety.
	for (auto iter = m_errorMessengers.begin(); iter != m_errorMessengers.end(); ++iter)
	{
		ErrorMessenger *pMessenger = (*iter);
		BGE_SAFE_DELETE(pMessenger);
	}
	m_errorMessengers.clear();
}

bool LogManager::Init(std::string_view configFileName)
{
	tinyxml2::XMLDocument xmlDocument; // Document object
	tinyxml2::XMLError xmlResult; // Result object

	xmlResult = xmlDocument.LoadFile(configFileName.data());
	if (xmlResult != tinyxml2::XML_SUCCESS) return false;

	// Fetch the root element: Logging
	auto *pRoot = xmlDocument.RootElement();
	if (!pRoot) return false;
	// Fetch the next most available child element
	auto *pElement = pRoot->FirstChildElement();
	if (!pElement) return false;

	static constexpr const char *c_kpATTRIB_TAG_NAME = "tag";
	static constexpr const char *c_kpATTRIB_CONSOLE_NAME = "useConsole";
	static constexpr const char *c_kpATTRIB_FILE_NAME = "useFile";
	while (pElement)
	{
		const std::string tagName(pElement->Attribute(c_kpATTRIB_TAG_NAME));
		
		if (!tagName.empty())
		{

		}
		// Try to find the next sibling element
		pElement = pElement->NextSiblingElement();
	}
	return true;
}

int LogManager::Write(std::string_view tagName, std::string_view msgFormat, va_list args)
{
	using BGE::GetSystemTimeString;
	// Just print for now
	const auto kTimeString = GetSystemTimeString();
	// Check for null optional
	if (!kTimeString)
	{
		return kNONE_WRITTEN;
	}
	// Calculate the required length of the formatted message
    va_list copyOfArgs;
    va_copy(copyOfArgs, args);
    const int kMsgLength = std::vsnprintf(nullptr, 0, msgFormat.data(), copyOfArgs);
    va_end(copyOfArgs);

    if (kMsgLength < 0)
    {
        return kNONE_WRITTEN;  // Error in formatting
    }

    // Create a buffer for the formatted message
    std::vector<char> buffer(kMsgLength + 1);
    std::vsnprintf(buffer.data(), buffer.size(), msgFormat.data(), args);
	
	std::ostringstream fmtStream; // Prepend items to arguments format
	fmtStream << *kTimeString << " [" << tagName << "] " << buffer.data() << '\n';
	
	const std::string kMessage = fmtStream.str();
	const std::string kTagNameStr = std::string(tagName);
	// Check if this message is a duplicate
	if (m_lastMessages.find(kTagNameStr) != m_lastMessages.end())
	{
		auto &lastMessage = m_lastMessages[kTagNameStr];
		if (lastMessage.message == kMessage)
		{
			// Increment the message count
			lastMessage.repeatCount++;
			return 0;
		}
		else
		{
			if (lastMessage.repeatCount > 1)
			{
				// TODO: Put this in a seperate member function
				std::fprintf(stdout, "%s [%s] %d duplicates detected, ommitting output...\n",
							 kTimeString.value().c_str(), kTagNameStr.c_str(), lastMessage.repeatCount);
			}
		}
	}
	// Log the current message
	const int kWritten = std::fprintf(stdout, "%s", kMessage.c_str());
	m_lastMessages[kTagNameStr] = { kMessage, 1 }; // Update last message at this tag
	return kWritten;
}

void LogManager::SetDisplayFlags(std::string_view tagName, std::uint8_t flags)
{
	// TODO: Implement thread safety.
	if (flags != 0)
	{
		auto resultIter = m_tags.find(std::string(tagName));
		if (resultIter == m_tags.end())
			m_tags.insert(std::make_pair(tagName, flags));
		else
			resultIter->second = flags;
	}
	else
	{
		m_tags.erase(std::string(tagName));
	}
}

void LogManager::AddErrorMessenger(Logger::ErrorMessenger *pMessenger)
{
	// TODO: Implement thread safety.
	m_errorMessengers.push_back(pMessenger);
}

LogManager::ErrorDialogResult LogManager::Error(Logger::ErrorMessenger &pMessenger, std::string_view tagName,
												std::string_view msgFormat ...)
{
	int buttonID = 0; // Message box result
	{
		SDL_MessageBoxData mbData;
		// SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT prevents the error icon from being shown.
		mbData.flags = SDL_MESSAGEBOX_ERROR;
		mbData.window = nullptr;
		mbData.title = "Error";

		// Just print for now
		const auto kTimeString = BGE::GetSystemTimeString();
		// Check for null optional
		if (!kTimeString)
			return ErrorDialogResult::kIgnore;

		std::ostringstream fmtStream; // Prepend items to arguments format
		fmtStream << *kTimeString << " [" << tagName << "] " << msgFormat << '\n';

		char buffer[512];
		
		va_list argList;
		va_start(argList, msgFormat);
		std::vsnprintf(buffer, 512, fmtStream.str().c_str(), argList);
		va_end(argList);
		
		mbData.message = buffer;
		mbData.numbuttons = 3;
		SDL_MessageBoxButtonData mbButtons[3];
		// Ignore button
		mbButtons[2].buttonID = 1;
		mbButtons[2].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		mbButtons[2].text = "Ignore";
		// Abort button
		mbButtons[1].buttonID = 2;
		mbButtons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
		mbButtons[1].text = "Abort";
		// Retry button
		mbButtons[0].buttonID = 3;
		mbButtons[0].flags = 0;
		mbButtons[0].text = "Retry";
		// Set buttons
		mbData.buttons = mbButtons;
		mbData.colorScheme = nullptr;

		SDL_ShowMessageBox(&mbData, &buttonID);
	}

	switch (buttonID)
	{
	case 1:
		return ErrorDialogResult::kIgnore;
		break;
	case 2:
		SDL_TriggerBreakpoint(); // Trigger a breakpoint when a debugger is attached
		return ErrorDialogResult::kAbort;
		break;
	case 3:
	default: // Cover default case as well with Retry result
		return ErrorDialogResult::kRetry;
		break;
	}
}
