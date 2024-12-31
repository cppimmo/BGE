/*=============================================================================*
 * EngineApp.cpp - Implementation of the application layer.
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
#include "Engine/EngineApp.hpp"

#include "Events/Events.hpp"
#include "Events/EventRegistry.hpp"
#include "MainLoop/Initialization.hpp"
#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"
#include "Memory/Memory.hpp"
#include "Resources/ResourceLoader.hpp"
#include "Scripting/ScriptExports.hpp"
#include "Audio/OpenALAudio.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <csignal>

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
	  m_bEditorRunning(false),
	  m_bResourceCheck(true)
{
	m_pMemoryManager = std::make_unique<MemoryManager>();
	m_pLocalizer = std::make_unique<Localizer>();
	m_pEventManager = std::make_unique<EventManager>("Global");
	m_pEventRegistry = std::make_unique<EventRegistry>("Global");
}

BGE::EngineApp::~EngineApp(void)
{
	// Call the OnShutdown routine if the game wasn't exited properly.
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

	// Queue event system started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_EventSystemStarted>());

	// Initialize the resource cache
	auto pResourceFile = std::make_shared<ZipResourceFile>(L"Assets.zip");
	m_pResourceCache = std::make_unique<ResourceCache>(50_MiB, pResourceFile);
	if (!m_pResourceCache->Init())
	{
		BGE_ERROR("Failed to initialize the resource cache!");
		return false;
	}

	// Register loaders for the resource cache
	// NOTE: Loaders should be registered from least to most specific.
	DefaultResourceLoaderFactory rlFactory;
	// XML resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateXMLResourceLoader());
	// JSON resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateJSONResourceLoader());
	// Script resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateScriptResourceLoader());
	// Image resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateBMPResourceLoader());
	m_pResourceCache->RegisterLoader(rlFactory.VCreateJPEGResourceLoader());
	m_pResourceCache->RegisterLoader(rlFactory.VCreatePNGResourceLoader());
	m_pResourceCache->RegisterLoader(rlFactory.VCreateTGAResourceLoader());
	// Shader resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateGLSLResourceLoader());
	// Sound resource loaders:
	m_pResourceCache->RegisterLoader(rlFactory.VCreateWAVResourceLoader());
	m_pResourceCache->RegisterLoader(rlFactory.VCreateOGGResourceLoader());

	// Queue resource cache started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_ResourceCacheStarted>());

	// Load localized strings:
	if (!m_pLocalizer->LoadStrings(Localizer::Language::kEnglish))
	{
		BGE_ERROR("Couldn't load localized strings!");
		return false;
	}
	// Queue localization started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_LocalizationStarted>());

	BGE_INFO("(ID_HOWDY): %s",  BGE::WStringToString(GetLocalizer().GetString(L"ID_HOWDY")).c_str());

	// TODO: Setup event manager.

	// Try to initialize the utility toolkit
	if (!BGUTInit("Engine.xml"))
	{
		BGE_ERROR("Couldn't initialize engine!");
		return false;
	}
	// Queue graphics start event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_GraphicsStarted>());

	// Set window title & icon
	BGUTSetWindowTitle(VGetGameTitle());
	BGUTSetWindowIcon(VGetIcon());

	// Initialize the audio system
	m_pAudioSystem = std::make_unique<OpenALAudioSystem>();
	if (!m_pAudioSystem->VInitialize())
	{
		BGE_ERROR("Failure to initialize the audio system!");
		return false;
	}
	// Queue sound system started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_SoundSystemStarted>());

	m_pGameLogic = VCreateGameAndView();
	if (!m_pGameLogic)
	{
		BGE_ERROR("BaseGameLogic Failure creating game & view");
		return false;
	}

	// Register script exports
    ScriptExports::Register();
	// Call IGameLogic::VPostInit after the scripting system is setup
	if (!m_pGameLogic->VPostInit())
	{
		BGE_ERROR("BaseGameLogic::VPostInit failure");
		return false;
	}

	// Queue scripting system started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_ScriptingSystemStarted>());

	// Queue game logic started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_GameLogicStarted>());

	// Initialize debug console
	m_pDebugConsole = std::make_unique<DebugConsole>();
#ifdef BGE_CONFIG_DEBUG
	// Enable debug console
	m_pDebugConsole->SetEnabled(true);
	// NOTE: The auto-complete isn't working properly at the moment.
	m_pDebugConsole->SetAutoCompleteEnabled(false);
#endif
	// Queue debug console started event
	BGE_QUEUE_GEVENT(std::make_shared<EventData_DebugConsoleStarted>());

	//BGE_LOG("Resources", "Num resources: %u", pResourceFile->VGetNumResources());
	//for (std::size_t i = 0; i < pResourceFile->VGetNumResources(); ++i)
	//{
	//	std::string name = pResourceFile->VGetResourceName(i);
	//	std::size_t s = pResourceFile->VGetRawResourceSize(Resource(name));
	//	BGE_LOG("Resources", "Resource(%u = %s, size=%u)", i, name.c_str(), s);
	//	if (name == "Assets\\Shaders\\test_vert.glsl")
	//	{
	//		char *pBuffer;
	//		std::size_t size = pResourceFile->VGetRawResource(Resource(name), pBuffer);
	//		BGE_LOG("Resources", "Resource(%u = %s, size=%u)", i, name.c_str(), size);
	//		BGE_LOG("Resources", "Resource(%s)", pBuffer);
	//	}
	//}
	//BGE_LOG("Resources", "test");
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

void BGE::EngineApp::OnRender(float deltaTime, float elapsedTime)
{
	auto &app = GetEngineApp();
	// TODO: Call rendering routines.

	// TODO: Replace with call to VPreRender().
	constexpr float kCLEAR_COLOR[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, kCLEAR_COLOR);

	// Render each game view
	auto &gameViews = app.GetGameLogic().GetGameViews();
	for (auto &pView : gameViews)
	{
		pView->VOnRender(deltaTime, elapsedTime);
	}

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
	// Call debug console ImGui routine
	app.GetDebugConsole().VImGuiRoutine();
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

void BGE::EngineApp::OnHandleSignal(int signal)
{
	switch (signal)
	{
	case SIGABRT:
		BGE_LOG("App", "Caught SIGABRT signal.");
		break;
	case SIGFPE:
		BGE_LOG("App", "Caught SIGFPE signal.");
		break;
	case SIGILL:
		BGE_LOG("App", "Caught SIGILL signal.");
		break;
	case SIGINT:
		BGE_LOG("App", "Caught SIGINT signal.");
		break;
	case SIGSEGV:
		BGE_LOG("App", "Caught SIGSEGV signal.");
		break;
	case SIGTERM:
		BGE_LOG("App", "Caught SIGTERM signal.");
		break;
	default:
		break;
	}

	// Handle specific cases
	switch (signal)
	{
	case SIGABRT:
	case SIGFPE:
	case SIGILL:
	case SIGSEGV:
		// These signals indicate a critical error and should terminate the app.
		std::exit(kBGE_EXIT_FAILURE);
		break;
	case SIGINT:
	case SIGTERM:
		// Attempt graceful shutdown (cleanup resources here if necessary)
		std::exit(kBGE_EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void BGE::EngineApp::OnShutdown(void)
{
	if (m_bHasQuit)
	{
		BGE_WARNING("Attempting to call EngineApp::OnShutdown multiple times");
		return;
	}
	//BGE_LOG("App", "Performing shutdown...");
	// TODO: Perform destruction tasks.
	VDestroyNetworkEventForwarder();

	ScriptExports::Deregister();

	m_bRunning = false;
	m_bHasQuit = true;
}

BGE::MemoryManager &BGE::EngineApp::GetMemoryManager(void) noexcept
{
	BGE_ASSERT(m_pMemoryManager);
	return *m_pMemoryManager.get();
}

BGE::Localizer &BGE::EngineApp::GetLocalizer(void) noexcept
{
	BGE_ASSERT(m_pLocalizer);
	return *m_pLocalizer.get();
}

BGE::EventManager &BGE::EngineApp::GetEventManager(void) noexcept
{
	BGE_ASSERT(m_pEventManager);
	return *m_pEventManager.get();
}

BGE::EventRegistry &BGE::EngineApp::GetEventRegistry(void) noexcept
{
	BGE_ASSERT(m_pEventRegistry);
	return *m_pEventRegistry.get();
}

BGE::BaseGameLogic &BGE::EngineApp::GetGameLogic(void) noexcept
{
	BGE_ASSERT(m_pGameLogic);
	return *m_pGameLogic.get();
}

BGE::ResourceCache &BGE::EngineApp::GetResourceCache(void) noexcept
{
	BGE_ASSERT(m_pResourceCache);
	return *m_pResourceCache.get();
}

BGE::DebugConsole &BGE::EngineApp::GetDebugConsole(void) noexcept
{
	BGE_ASSERT(m_pDebugConsole);
	return *m_pDebugConsole.get();
}

BGE::IAudioSystem &BGE::EngineApp::GetAudioSystem(void) noexcept
{
	BGE_ASSERT(m_pAudioSystem);
	return *m_pAudioSystem.get();
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
	BGE_REGISTER_EVENT(EventData_DebugConsoleStarted);
	BGE_REGISTER_EVENT(EventData_EventSystemStarted);
	BGE_REGISTER_EVENT(EventData_GameLogicStarted);
	BGE_REGISTER_EVENT(EventData_GamePaused);
	BGE_REGISTER_EVENT(EventData_GameResumed);
	BGE_REGISTER_EVENT(EventData_GraphicsStarted);
	BGE_REGISTER_EVENT(EventData_LocalizationStarted);
	BGE_REGISTER_EVENT(EventData_NetworkStarted);
	BGE_REGISTER_EVENT(EventData_PhysicsStarted);
	BGE_REGISTER_EVENT(EventData_ResourceCacheStarted);
	BGE_REGISTER_EVENT(EventData_ScriptingSystemStarted);
	BGE_REGISTER_EVENT(EventData_SoundSystemStarted);
}
