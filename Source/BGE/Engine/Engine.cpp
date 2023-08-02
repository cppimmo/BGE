/*=============================================================================*
 * Engine.cpp - Implementation of the application layer.
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
#include "Engine/EngineStd.hpp"
#include "Engine/Engine.hpp"

#include "MainLoop/Initialization.hpp"
#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// Initialize global application instance pointer
extern BGE::UniqueEngineAppPtr BGE::g_pApp = nullptr;

BGE::UniqueEngineAppPtr &BGE::GetEngineAppPtr(void)
{
	return g_pApp;
}

BGE::EngineApp::EngineApp(void)
	: m_isRunning(false),
	  m_quitRequested(false),
	  m_quitting(false),
	  m_hasQuit(false),
	  m_isEditorRunning(false)
{
}

BGE::EngineApp::~EngineApp(void)
{
	// Call the OnClose routine if the game wasn't exited properly.
	if (!m_hasQuit)
		OnClose();
}

bool BGE::EngineApp::LoadStrings(std::string_view language)
{
	using namespace tinyxml2;

	std::string languageFilePath = "Strings/";
	languageFilePath += language; // Specify language
	languageFilePath += ".xml";

	XMLDocument xmlDocument; // Document object
	XMLError xmlResult; // Result object

	xmlResult = xmlDocument.LoadFile(languageFilePath.c_str());
	if (xmlResult != XML_SUCCESS)
	{
		BGE_ERROR("Strings are missing.");
		return false;
	}

	auto *pRoot = xmlDocument.RootElement(); // Fetch the root element: Strings
	if (!pRoot) return false;
	
	for (auto pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
	{
		const char *pKey = pElem->Attribute("sID");
		const char *pText = pElem->Attribute("value");

		if (pKey && pText)
		{
			// Convert both attributes to wide character strings
			auto wideKey = StringToWString(pKey);
			auto wideText = StringToWString(pText);
			// Set the text for the current ID:
			m_textStrings[wideKey] = wideText;
		}
	}
	return true;
}

std::wstring BGE::EngineApp::GetString(std::wstring_view sID)
{
	auto localizedStringIter = m_textStrings.find(sID.data());
	if (localizedStringIter == m_textStrings.end())
	{
		BGE_ASSERT(0 && "String not found!");
		return std::wstring();
	}
	return localizedStringIter->second; // Return contents at sID
}

void BGE::EngineApp::OnUpdate(float deltaTime, float elapsedTime)
{
	// TODO: Call update routines.
}

void BGE::EngineApp::OnRender(void)
{
	// TODO: Call rendering routines.
}

void BGE::EngineApp::OnHandleEvent(const SDL_Event &event)
{
	// TODO: Handle necessary SDL events.
	/*switch (event)
	{
	default:
		break;
	}*/
}

void BGE::EngineApp::OnDisplayChange(int colorDepth, int width, int height)
{
	// TODO: Implement code which operates on renderer for display change.
}

void BGE::EngineApp::OnClose(void)
{
	// TODO: Perform destruction tasks.
}

bool BGE::EngineApp::VLoadGame(void)
{
	return false;
}
// Method named InitInstance since only one instance of the game is allowed in most cases.
bool BGE::EngineApp::VInitInstance(void)
{
	// TODO: Implement intialization tasks.
#ifdef BGE_CONFIG_DEBUG
	HideConsole(); // TODO: This should be called by Logger based on configuration.
#endif
	if (!IsDiskSpaceAvailable(1'000))
	{
		BGE_ERROR("Not enough storage!");
		return false;
	}

	if (!IsMemoryAvailable(1'000))
	{
		BGE_ERROR("Not enough memory!");
		return false;
	}
#ifdef BGE_CONFIG_DEBUG
	BGE_INFO("Platform: %s", GetPlatform().data());
	BGE_INFO("CPU speed: %dMHz", ReadCPUSpeed());
	BGE_INFO("Logical CPU cores: %d", ReadLogicalCPUCores());
#endif
#ifdef BGE_CONFIG_RELEASE
	if (!IsOnlyInstance("BGE"))
	{
		BGE_ERROR("Only one instance of the application is allowed!");
		return false;
	}

	if (!IsDebuggerPresent())
		return false;
#endif
	// Load localized strings:
	if (!LoadStrings("English"))
	{
		BGE_ERROR("Couldn't load localized strings!");
		return false;
	}

	return true;
}

void BGE::EngineApp::VRegisterGameEvents(void)
{
}

void BGE::EngineApp::VCreateNetworkEventForwarder(void)
{
}

void BGE::EngineApp::VDestroyNetworkEventForwarder(void)
{
}

void BGE::EngineApp::RegisterEngineEvents(void)
{
}
