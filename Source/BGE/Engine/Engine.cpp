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

#include "Events/Events.hpp"
#include "Events/EventRegistry.hpp"
#include "MainLoop/Initialization.hpp"
#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Graphics/Screenshot.hpp"

// Initialize global application instance pointer
BGE::UniqueEngineAppPtr BGE::g_pApp = nullptr;

BGE::EngineApp &BGE::GetEngineApp(void)
{
	return *g_pApp;
}

BGE::EngineApp::EngineApp(void)
	: m_bRunning(false),
	  m_bQuitRequested(false),
	  m_bQuitting(false),
	  m_bHasQuit(false),
	  m_bEditorRunning(false)
{
	m_pLocalizer = std::make_unique<Localizer>();
	m_pEventManager = std::make_unique<EventManager>("Global");
	m_pEventRegistry = std::make_unique<EventRegistry>("Global");
}

BGE::EngineApp::~EngineApp(void)
{
	// Call the OnClose routine if the game wasn't exited properly.
	if (!m_bHasQuit)
		OnShutdown();
}

// Method named InitInstance since only one instance of the game is allowed in most cases.
bool BGE::EngineApp::VInitInstance(void)
{
	// TODO: Implement intialization tasks.
#ifdef BGE_CONFIG_DEBUG
	HideConsole(); // TODO: This should be called by Logger based on configuration.
#endif
	if (m_bResourceCheck)
	{
		constexpr int kDISK_SPACE_MIB = 1'000;
		constexpr int kMEMORY_MIB = 1'000;

		if (!IsDiskSpaceAvailable(kDISK_SPACE_MIB))
		{
			BGE_ERROR("Not enough storage!");
			return false;
		}

		if (!IsMemoryAvailable(kMEMORY_MIB))
		{
			BGE_ERROR("Not enough memory!");
			return false;
		}
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
	// Register all events
    RegisterEngineEvents();
    VRegisterGameEvents();
	// Log the registered events
	m_pEventRegistry->LogRegisteredEvents();

	BGE_QUEUE_GEVENT(std::make_shared<EventData_EventSystemStarted>());

	// Load localized strings:
	if (!m_pLocalizer->LoadStrings(Localizer::Language::kEnglish))
	{
		BGE_ERROR("Couldn't load localized strings!");
		return false;
	}
	BGE_QUEUE_GEVENT(std::make_shared<EventData_LocalizationStarted>());

	BGE_INFO("(ID_HOWDY): %s",  BGE::WStringToString(GetLocalizer().GetString(L"ID_HOWDY")).c_str());

	// TODO: Setup event manager.

	// Try to initialize the utility toolkit
	if (!BGUTInit("Engine.xml"))
	{
		BGE_ERROR("Couldn't initialize engine!");
		return false;
	}
	BGE_QUEUE_GEVENT(std::make_shared<EventData_GraphicsStarted>());

	BGUTSetWindowTitle(VGetGameTitle());

	m_pGameLogic = VCreateGameAndView();
	if (!m_pGameLogic) return false;
	BGE_QUEUE_GEVENT(std::make_shared<EventData_GameLogicStarted>());

	m_bRunning = true;

	return true;
}

void BGE::EngineApp::OnUpdate(float deltaTime, float elapsedTime)
{
	// TODO: Call update routines.
	auto &app = GetEngineApp();
	// TODO: Update event queue.
	// TODO: Update network stuff.
	app.m_pGameLogic->VOnUpdate(deltaTime, elapsedTime);
	// TODO: Set a reasonable event queue processing timeout.
	// Allow event queue to process for up to ___ milliseconds
	app.m_pEventManager->VUpdate(EventManager::kINFINITY);
}

void BGE::EngineApp::OnRender(void)
{
	auto &app = GetEngineApp();

	// TODO: Call rendering routines.

	constexpr float kCLEAR_COLOR[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, kCLEAR_COLOR);

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
}

bool BGE::EngineApp::OnHandleEvent(const SDL_Event &kEvent)
{
	bool bResult = false;
	auto &app = GetEngineApp();
	// TODO: Handle necessary SDL events.
	switch (kEvent.type)
	{
	case SDL_QUIT:
		app.OnShutdown();
		bResult = true;
		break;
	case SDL_APP_TERMINATING:
		break;
	case SDL_APP_LOWMEMORY:
		break;
	case SDL_APP_WILLENTERBACKGROUND:
		break;
	case SDL_APP_DIDENTERBACKGROUND:
		break;
	case SDL_APP_WILLENTERFOREGROUND:
		break;
	case SDL_APP_DIDENTERFOREGROUND:
		break;
	case SDL_LOCALECHANGED:
		break;
	case SDL_DISPLAYEVENT:
		break;
	case SDL_WINDOWEVENT:
		break;
	case SDL_SYSWMEVENT:
		break;
	// Remaining cases forwarded to GameLogic/GameViews:
	case SDL_KEYDOWN: // Keyboard events
	case SDL_KEYUP:
	case SDL_TEXTEDITING:
	case SDL_TEXTINPUT:
	case SDL_KEYMAPCHANGED:
	case SDL_TEXTEDITING_EXT:
	case SDL_MOUSEMOTION: // Mouse events
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEWHEEL:
	case SDL_JOYAXISMOTION: // Joystick events
	case SDL_JOYBALLMOTION:
	case SDL_JOYHATMOTION:
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	case SDL_JOYDEVICEADDED:
	case SDL_JOYDEVICEREMOVED:
	case SDL_JOYBATTERYUPDATED:
	case SDL_CONTROLLERAXISMOTION: // Game controller events
	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
	case SDL_CONTROLLERDEVICEADDED:
	case SDL_CONTROLLERDEVICEREMOVED:
	case SDL_CONTROLLERDEVICEREMAPPED:
	case SDL_CONTROLLERTOUCHPADDOWN:
	case SDL_CONTROLLERTOUCHPADMOTION:
	case SDL_CONTROLLERTOUCHPADUP:
	case SDL_CONTROLLERSENSORUPDATE:
	case SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3:
	case SDL_CONTROLLERSTEAMHANDLEUPDATED:
	case SDL_FINGERDOWN: // Touch events
	case SDL_FINGERUP:
	case SDL_FINGERMOTION:
	case SDL_DOLLARGESTURE: // Gesture events
	case SDL_DOLLARRECORD:
	case SDL_MULTIGESTURE:
	case SDL_CLIPBOARDUPDATE: // Clipboard events
	case SDL_DROPFILE: // Drag and drop events
	case SDL_DROPTEXT:
	case SDL_DROPBEGIN:
	case SDL_DROPCOMPLETE:
	case SDL_AUDIODEVICEADDED: // Audio hotplug events
	case SDL_AUDIODEVICEREMOVED:
	{
		auto &gameViews = app.GetGameLogic().GetGameViews();
		// Iterate through the game views in reverse
		for (auto it = gameViews.rbegin(); it != gameViews.rend(); ++it)
		{
			BGE_LOG("SDL Events", "Sending event to game view");
			if ((*it)->VOnHandleEvent(kEvent))
			{
				bResult = true;
				break; // Breaks out of loop
			}
		}
		break;
	}
	default:
		break;
	}
	return bResult;
}

void BGE::EngineApp::OnDisplayChange(int colorDepth, int width, int height)
{
	// TODO: Implement code which operates on renderer for display change.
}

void BGE::EngineApp::OnShutdown(void)
{
	// TODO: Perform destruction tasks.
}

BGE::Localizer &BGE::EngineApp::GetLocalizer(void)
{
	BGE_ASSERT(m_pLocalizer);
	return *m_pLocalizer.get();
}

BGE::EventManager &BGE::EngineApp::GetEventManager(void)
{
	BGE_ASSERT(m_pEventManager);
	return *m_pEventManager.get();
}

BGE::EventRegistry &BGE::EngineApp::GetEventRegistry(void)
{
	BGE_ASSERT(m_pEventRegistry);
	return *m_pEventRegistry.get();
}

BGE::BaseGameLogic &BGE::EngineApp::GetGameLogic(void)
{
	BGE_ASSERT(m_pGameLogic);
	return *m_pGameLogic.get();
}

int BGE::EngineApp::GetExitCode(void) const
{
	return BGUTGetExitCode();
}

bool BGE::EngineApp::VLoadGame(void)
{
	return false;
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
	BGE_REGISTER_EVENT(EventData_EventSystemStarted);
	BGE_REGISTER_EVENT(EventData_GameLogicStarted);
	BGE_REGISTER_EVENT(EventData_GamePaused);
	BGE_REGISTER_EVENT(EventData_GameResumed);
	BGE_REGISTER_EVENT(EventData_GraphicsStarted);
	BGE_REGISTER_EVENT(EventData_LocalizationStarted);
	BGE_REGISTER_EVENT(EventData_NetworkStarted);
	BGE_REGISTER_EVENT(EventData_PhysicsStarted);
	BGE_REGISTER_EVENT(EventData_ResourceCacheStarted);
	BGE_REGISTER_EVENT(EventData_SoundSystemStarted);
}
