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
#include <list>

using namespace BGE;

// Singleton
class LogManager;
static LogManager *s_pLogManager = nullptr;

class LogManager
{
public:
	enum struct ErrorDialogResult
	{
		Abort,
		Retry,
		Ignore
	};
	using TagMap = std::map<std::string, std::uint8_t>;
	using ErrorMessengerList = std::list<Logger::ErrorMessenger *>;
private:
	TagMap m_tags;
	ErrorMessengerList m_errorMessengers;
	// thread mutexes:
public:
	LogManager(void);
	LogManager(const LogManager &) = delete;
	LogManager &operator=(const LogManager &) = delete;
	LogManager(LogManager &&) noexcept = delete;
	LogManager &operator=(LogManager &&) noexcept = delete;
	~LogManager(void);

	bool Init(std::string_view configFilename);
	int Write(std::string_view tagName, std::string_view msgFormat, ...);
	void SetDisplayFlags(std::string_view tagName, std::uint8_t flags);
	void AddErrorMessenger(Logger::ErrorMessenger *pMessenger);
	ErrorDialogResult Error(Logger::ErrorMessenger &pMessenger, std::string_view tagName, std::string_view msgFormat...);
private:
};

Logger::ErrorMessenger::ErrorMessenger(bool isFatal)
	: m_isEnabled(true),
	  m_isFatal(isFatal)
{
	::s_pLogManager->AddErrorMessenger(this);
}

int Logger::ErrorMessenger::Show(std::string_view tagName, std::string_view msgFormat, ...)
{
	if (m_isEnabled)
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
	return m_isEnabled;
}

bool Logger::ErrorMessenger::Fatal(void) const noexcept
{
	return m_isFatal;
}

void Logger::Init(std::string_view configFilename)
{
	if (!::s_pLogManager)
	{
		::s_pLogManager = BGE_NEW LogManager;
		(void)::s_pLogManager->Init(configFilename);
	}
}

void Logger::Destroy(void)
{
	SAFE_DELETE(::s_pLogManager);
}

int Logger::Write(std::string_view tagName, std::string_view msgFormat, ...)
{
	using BGE::Utils::GetSystemTimeString;
	// just print for now
	const auto kTimeString = GetSystemTimeString();
	// check for null optional
	if (!kTimeString)
		return -1;

	std::ostringstream fmtStream; // prepend items to arguments format
	fmtStream << *kTimeString << " [" << tagName << "] " << msgFormat << '\n';

	va_list pArgList;
	va_start(pArgList, msgFormat);
	std::vfprintf(stdout, fmtStream.str().c_str(), pArgList);
	va_end(pArgList);
	return 0;
}

void Logger::SetMaxMessageLength(std::size_t length)
{
}

void Logger::SetDisplayFlags(std::string_view tagName, std::uint8_t flags)
{
	BGE_ASSERT(::s_pLogManager);
	::s_pLogManager->SetDisplayFlags(tagName, flags);
}

void BGE::Logger::LogOutputFunc_SDL(void *pUserData, int category, SDL_LogPriority priority, const char *pMessage)
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
		SAFE_DELETE(pMessenger);
	}
	m_errorMessengers.clear();
}

bool LogManager::Init(std::string_view configFilename)
{
	using namespace tinyxml2;
	XMLDocument xmlDocument; // Document object
	XMLError xmlResult; // Result object

	xmlResult = xmlDocument.LoadFile(configFilename.data());
	if (xmlResult != XML_SUCCESS) return false;

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

int LogManager::Write(std::string_view tagName, std::string_view msgFormat, ...)
{
	return 0;
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
	int buttonId = 0; // Message box result
	{
		SDL_MessageBoxData mbData;
		// SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT prevents the error icon from being shown.
		mbData.flags = SDL_MESSAGEBOX_ERROR;
		mbData.window = nullptr;
		mbData.title = "Error";

		// just print for now
		const auto kTimeString = BGE::Utils::GetSystemTimeString();
		// check for null optional
		if (!kTimeString)
			return ErrorDialogResult::Ignore;

		std::ostringstream fmtStream; // prepend items to arguments format
		fmtStream << *kTimeString << " [" << tagName << "] " << msgFormat << '\n';

		char buffer[512];
		
		va_list pArgList;
		va_start(pArgList, msgFormat);
		std::vsnprintf(buffer, 512, fmtStream.str().c_str(), pArgList);
		va_end(pArgList);
		
		mbData.message = buffer;
		mbData.numbuttons = 3;
		SDL_MessageBoxButtonData mbButtons[3];
		// Ignore button
		mbButtons[2].buttonid = 1;
		mbButtons[2].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		mbButtons[2].text = "Ignore";
		// Abort button
		mbButtons[1].buttonid = 2;
		mbButtons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
		mbButtons[1].text = "Abort";
		// Retry button
		mbButtons[0].buttonid = 3;
		mbButtons[0].flags = 0;
		mbButtons[0].text = "Retry";
		// Set buttons
		mbData.buttons = mbButtons;
		mbData.colorScheme = nullptr;

		SDL_ShowMessageBox(&mbData, &buttonId);
	}

	switch (buttonId)
	{
	case 1:
		return ErrorDialogResult::Ignore;
		break;
	case 2:
		SDL_TriggerBreakpoint(); // Trigger a breakpoint when a debugger is attached
		return ErrorDialogResult::Abort;
		break;
	case 3:
	default: // Cover default case as well with Retry result
		return ErrorDialogResult::Retry;
		break;
	}
}

