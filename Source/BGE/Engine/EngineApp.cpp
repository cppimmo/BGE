/*******************************************************************************
 * @file   EngineApp.cpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Implementation of the application layer.
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
#include "Engine/EngineApp.hpp"

#include "Events/Events.hpp"
#include "Events/EventRegistry.hpp"
#include "MainLoop/Initialization.hpp"
#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"
#include "Memory/Memory.hpp"
#include "Resources/ResourceLoader.hpp"
#include "Scripting/ScriptExports.hpp"
#include "Audio/AL/AudioSystem.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <csignal>

namespace BGE
{
	// Initialize global application instance pointer
	UniqueEngineAppPtr g_pApp = nullptr;

	EngineApp &GetEngineApp(void)
	{
		return *g_pApp;
	}

	EngineApp::EngineApp(void)
		: m_bRunning(false),
		  m_bQuitRequested(false),
		  m_bQuitting(false),
		  m_bHasQuit(false),
		  m_bEditorRunning(false),
		  m_bResourceCheck(true)
	{
		m_pMemoryManager = std::make_unique<MemoryManager>(64_MiB);
		m_pLocalizer = std::make_unique<Localizer>();
		m_pEventManager = std::make_unique<EventManager>("Global");
		m_pEventRegistry = std::make_unique<EventRegistry>("Global");
	}

	EngineApp::~EngineApp(void)
	{
		// Call the OnShutdown routine if the game wasn't exited properly.
		OnShutdown();
	}

	// Method named InitInstance since only one instance of the game is allowed in most cases.
	bool EngineApp::VInitInstance(void)
	{
		m_timer.Start(); // Start the timer to measure the initialization time
		BGE_LOG("App", "Beginning initialization...");
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
		{
			return false;
		}
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

		BGE_INFO("(ID_HOWDY): %s", GetLocalizer().GetString("ID_HOWDY").c_str());

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
		std::string iconFileName;
		if (!(iconFileName = VGetIcon()).empty())
		{
			BGUTSetWindowIcon(iconFileName);
		}

		// Initialize the audio system
		m_pAudioSystem = std::make_unique<OpenALAudioSystem>();
		if (!m_pAudioSystem->VInit())
		{
			BGE_ERROR("Failure to initialize the audio system!");
			return false;
		}
		BGE_LOG("Audio", "Current implementation: %s", AudioImplToString(m_pAudioSystem->VGetImpl()).data());
		// Queue sound system started event
		BGE_QUEUE_GEVENT(std::make_shared<EventData_SoundSystemStarted>());

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

		m_pGameLogic = VCreateGameAndView();
		if (!m_pGameLogic)
		{
			BGE_ERROR("BaseGameLogic Failure creating game & view");
			return false;
		}
		// Register script exports
		ScriptExports::Register();

		// Call IGameLogic::VPostInit after the scripting system is setup & scripts have run
		if (!m_pGameLogic->VPostInit())
		{
			BGE_ERROR("BaseGameLogic::VPostInit failure");
			return false;
		}

		// Queue scripting system started event
		BGE_QUEUE_GEVENT(std::make_shared<EventData_ScriptingSystemStarted>());

		// Queue game logic started event
		BGE_QUEUE_GEVENT(std::make_shared<EventData_GameLogicStarted>());

#if 0
		BGE_LOG("Resources", "Num resources: %u", pResourceFile->VGetNumResources());
		for (std::size_t i = 0; i < pResourceFile->VGetNumResources(); ++i)
		{
			std::string name = pResourceFile->VGetResourceName(i);
			std::size_t s = pResourceFile->VGetRawResourceSize(Resource(name));
			BGE_LOG("Resources", "Resource(%u = %s, size=%u)", i, name.c_str(), s);
			if (name == "Assets\\Shaders\\test_vert.glsl")
			{
				char *pBuffer;
				std::size_t size = pResourceFile->VGetRawResource(Resource(name), pBuffer);
				BGE_LOG("Resources", "Resource(%u = %s, size=%u)", i, name.c_str(), size);
				BGE_LOG("Resources", "Resource(%s)", pBuffer);
			}
		}
		BGE_LOG("Resources", "test");
#endif
		m_bRunning = true;

		BGE_LOG("App", "Initialized successfully");

		const Timer::Seconds seconds = m_timer.GetElapsedSecs();
		BGE_LOG("App", "Initialization duration: %.2f seconds", seconds);
		m_timer.Reset(); // Reset the application timer for the main loop
		return true;
	}

	void EngineApp::OnUpdate(float deltaTime, float elapsedTime)
	{
		auto &app = GetEngineApp();

		app.UpdateFPS(); // Calculate the FPS
		// TODO: Update network stuff.

		// Update the game logic
		app.m_pGameLogic->VOnUpdate(deltaTime, elapsedTime);

		// TODO: Set a reasonable event queue processing timeout.
		// Update the event queue
		// Allow event queue to process for up to ___ milliseconds
		app.m_pEventManager->VUpdate(EventManager::kINFINITY);
	}

	void EngineApp::OnRender(float deltaTime, float elapsedTime)
	{
		auto &app = GetEngineApp();
		// TODO: Call rendering routines.

		// TODO: Replace with call to VPreRender().
		float clearColor[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
		auto time = app.GetTimer().GetElapsedSecs();
		clearColor[0] = (std::sin(time * 0.5f) + 1.0f) / 2.0f;
		clearColor[1] = (std::sin(time * 0.3f) + 1.0f) / 2.0f;
		clearColor[2] = (std::sin(time * 0.7f) + 1.0f) / 2.0f;
		glClearBufferfv(GL_COLOR, 0, clearColor);

		// Render each game view
		auto &gameViews = app.GetGameLogic().GetGameViews();
		for (auto &pView : gameViews)
		{
			pView->VOnRender(deltaTime, elapsedTime);
		}

		ImGui::Begin("FPS");
		ImGui::Text("%03.4f", app.GetFPSData().smoothedFPS);
		ImGui::End();

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
		// Call debug console ImGui routine
		app.GetDebugConsole().VImGuiRoutine();
	}

	bool EngineApp::OnHandleEvent(const SDL_Event &kEvent)
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

	void EngineApp::OnDisplayChange(int colorDepth, int width, int height)
	{
		// TODO: Implement code which operates on renderer for display change.
	}

	void EngineApp::OnHandleSignal(int signal)
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
			BGE_LOG("App", "Shutting down due to caught signal (critical)");
			// These signals indicate a critical error and should terminate the app.
			std::exit(kBGE_EXIT_FAILURE);
			break;
		case SIGINT:
		case SIGTERM:
			BGE_LOG("App", "Shutting down due to caught signal (non-critical)");
			// Attempt graceful shutdown (cleanup resources here if necessary)
			std::exit(kBGE_EXIT_SUCCESS);
			break;
		default:
			break;
		}
	}

	void EngineApp::OnShutdown(void)
	{
		if (m_bHasQuit)
		{
			BGE_WARNING("Attempting to call EngineApp::OnShutdown multiple times");
			return;
		}
		BGE_LOG("App", "Performing shutdown...");

		m_timer.Stop(); // Stop the application timer
		const Timer::Seconds seconds = m_timer.GetElapsedSecs();
		BGE_LOG("App", "Main loop duration: %.2f seconds", seconds);
		// TODO: Perform destruction tasks.
		VDestroyNetworkEventForwarder();

		ScriptExports::Deregister();

		m_bRunning = false;
		m_bHasQuit = true;
	}

	const Timer &EngineApp::GetTimer(void) const noexcept
	{
		return m_timer;
	}

	const EngineApp::FPSData &EngineApp::GetFPSData(void) const noexcept
	{
		return m_fpsData;
	}

	MemoryManager &EngineApp::GetMemoryManager(void) noexcept
	{
		BGE_ASSERT(m_pMemoryManager);
		return *m_pMemoryManager.get();
	}

	Localizer &EngineApp::GetLocalizer(void) noexcept
	{
		BGE_ASSERT(m_pLocalizer);
		return *m_pLocalizer.get();
	}

	EventManager &EngineApp::GetEventManager(void) noexcept
	{
		BGE_ASSERT(m_pEventManager);
		return *m_pEventManager.get();
	}

	EventRegistry &EngineApp::GetEventRegistry(void) noexcept
	{
		BGE_ASSERT(m_pEventRegistry);
		return *m_pEventRegistry.get();
	}

	BaseGameLogic &EngineApp::GetGameLogic(void) noexcept
	{
		BGE_ASSERT(m_pGameLogic);
		return *m_pGameLogic.get();
	}

	ResourceCache &EngineApp::GetResourceCache(void) noexcept
	{
		BGE_ASSERT(m_pResourceCache);
		return *m_pResourceCache.get();
	}

	DebugConsole &EngineApp::GetDebugConsole(void) noexcept
	{
		BGE_ASSERT(m_pDebugConsole);
		return *m_pDebugConsole.get();
	}

	IAudioSystem &EngineApp::GetAudioSystem(void) noexcept
	{
		BGE_ASSERT(m_pAudioSystem);
		return *m_pAudioSystem.get();
	}

	int EngineApp::GetExitCode(void) const
	{
		return BGUTGetExitCode();
	}

	bool EngineApp::VLoadGame(void)
	{
		return false;
	}

	void EngineApp::VRegisterGameEvents(void)
	{
	}

	void EngineApp::VCreateNetworkEventForwarder(void)
	{
	}

	void EngineApp::VDestroyNetworkEventForwarder(void)
	{
	}

	void EngineApp::RegisterEngineEvents(void)
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

	void EngineApp::UpdateFPS(void)
	{
		// Get the current elapsed time in milliseconds
		const Timer::Milliseconds kCurrElapsedMS = m_timer.GetElapsedMillis();

		// Calculate time for this frame
		const Timer::Milliseconds kFrameTime = kCurrElapsedMS - m_fpsData.lastElapsedMS;
		m_fpsData.lastElapsedMS = kCurrElapsedMS;

		// Update the circular buffer and rolling sum
		m_fpsData.sumFrameTimes -= m_fpsData.frameTimes[m_fpsData.currentFrameIndex]; // Remove old frame time
		m_fpsData.frameTimes[m_fpsData.currentFrameIndex] = kFrameTime; // Add new frame time
		m_fpsData.sumFrameTimes += kFrameTime; // Update sum

		// Update circular buffer index
		m_fpsData.currentFrameIndex = (m_fpsData.currentFrameIndex + 1) % FPSData::kFRAME_SAMPLE_COUNT;

		// Calculate smoothed FPS (avoid division by zero)
		if (m_fpsData.sumFrameTimes > 0)
		{
			m_fpsData.smoothedFPS = 1'000.0f * FPSData::kFRAME_SAMPLE_COUNT / m_fpsData.sumFrameTimes;
		}
		else
		{
			m_fpsData.smoothedFPS = 0.0f; // Fallback to 0 FPS if no valid frame times
		}
	}
} // End namespace (BGE)
