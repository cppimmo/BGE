/*******************************************************************************
 * @file   EngineApp.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Declaration of the application layer.
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
#ifndef _BGE_ENGINEAPP_HPP_
#define _BGE_ENGINEAPP_HPP_

//#include <RmlUi/Core.h>

#include "MainLoop/Initialization.hpp"
#include "Utilities/Types.hpp"
#include "Engine/EngineOptions.hpp"
#include "Engine/BaseGameLogic.hpp"
#include "Events/EventManager.hpp"
#include "Events/EventRegistry.hpp"
#include "Resources/Localizer.hpp"
#include "Resources/ResourceCache.hpp"
#include "Memory/Memory.hpp"
#include "UI/DebugConsole.hpp"
#include "Audio/Audio.hpp"
#include "Graphics/Renderer.hpp"

namespace BGE
{
	// Set global application pointer to the instance of an app layer class.
	extern BGE_ENGINE_API UniqueEngineAppPtr g_pApp; // Global application layer pointer (use as singleton)
	
	//! Return reference to the global EngineApp instance.
	EngineApp &GetEngineApp(void);
	// Call in game programs to create an instance of the EngineApp.
	template <typename DerivedApp, typename... Args>
	bool CreateEngineApp(Args... args) requires(std::derived_from<DerivedApp, EngineApp>)
	{
		if (!g_pApp)
		{
			g_pApp = std::make_unique<DerivedApp>(args...);
		}
		else
		{
			BGE_ASSERT(false && "Attempted to create multiple app instances!");
			return false;
		}
		return g_pApp.operator bool();
	}

	/**
	 * @brief Application layer class.
	 */
	class EngineApp : public INonCopyable, public INonMovable
	{
	public:
		//! Struct for holding data used to calculate the FPS.
		struct FPSData
		{
			static constexpr int kFRAME_SAMPLE_COUNT = 60; //!< Number of samples for averaging.
			std::array<Timer::Milliseconds, kFRAME_SAMPLE_COUNT> frameTimes = { 0 }; //!< Store frame durations.
			int currentFrameIndex = 0; //!< Current index in the circular buffer.
			Timer::Milliseconds lastElapsedMS = 0; //!< Time at the last frame.
			Timer::Milliseconds sumFrameTimes = 0; //!< Rolling sum of frame times.
			float smoothedFPS = 0.0f; //!< Calculated FPS.
		};
	protected:
		EngineOptions m_options; //!< Engine app layer options.
		bool m_bRunning; //!< True if game is in the main loop.
		bool m_bQuitRequested; //!< True if the exit sequence is nigh.
		bool m_bQuitting; //!< True if the exit sequence is being ran.
		bool m_bHasQuit; //!< True if the exit sequence has been run.
		bool m_bEditorRunning; //!< True if the game editor is running.
		bool m_bResourceCheck; //!< Check system resources for availability.

		Timer m_timer; //!< Application timer.
		FPSData m_fpsData; //!< Data used to calculate the FPS.

		UniqueMemoryManagerPtr m_pMemoryManager; //!< Primary memory manager.
		UniqueLocalizerPtr m_pLocalizer; //!< Localization handler.
		UniqueIEventManagerPtr m_pEventManager; //!< Main event manager.
		UniqueEventRegistryPtr m_pEventRegistry; //!< Main event registrar.
		UniqueBaseGameLogicPtr m_pGameLogic; //!< Game logic.
		UniqueResourceCachePtr m_pResourceCache; //!< Primary resource cache.
		UniqueDebugConsolePtr m_pDebugConsole; //!< Engine debug console.
		UniqueIAudioSystemPtr m_pAudioSystem; //!< Audio system.
		UniqueIRendererPtr m_pRenderer; //!< Renderer.

		// TODO: Add base socket manager.
		// TODO: Add network event forwarder.

		//Rml::Context *m_pRmlContext = nullptr;
	public:
		EngineApp(void);
		virtual ~EngineApp(void);

		// EngineApp interface:
		virtual bool VInitInstance(const EngineOptions &kOptions, const std::filesystem::path &kConfigPath);
		virtual UniqueBaseGameLogicPtr VCreateGameAndView(void) = 0;
		virtual bool VLoadGame(void);
		virtual std::string VGetGameTitle(void) = 0; // Application related data
		virtual std::string VGetGameAppDirectory(void) = 0;
		virtual std::string VGetIcon(void) = 0; // Icon filename
		virtual int VGetRendererImpl(void) = 0; // Type of renderer

		static void OnUpdate(float deltaTime, float elapsedTime);
		static void OnRender(float deltaTime, float elsapsedTime);
		static bool OnEvent(const SDL_Event &kEvent);
		static void OnResize(std::int32_t width, std::int32_t height);
		// Application signal handlers:
		static void OnSignal(int signal);
		void OnShutdown(void);
		// Accessors:
		const EngineOptions &GetOptions(void) const noexcept;
		const Timer &GetTimer(void) const noexcept;
		const FPSData &GetFPSData(void) const noexcept;
		MemoryManager &GetMemoryManager(void) noexcept;
		Localizer     &GetLocalizer(void) noexcept;
		IEventManager &GetEventManager(void) noexcept;
		EventRegistry &GetEventRegistry(void) noexcept;
		BaseGameLogic &GetGameLogic(void) noexcept;
		ResourceCache &GetResourceCache(void) noexcept;
		DebugConsole  &GetDebugConsole(void) noexcept;
		IAudioSystem  &GetAudioSystem(void) noexcept;
		IRenderer     &GetRenderer(void) noexcept;
		StrongIGameViewPtr GetHumanView(std::size_t index = 0);
		std::size_t GetHumanViewCount(void) const noexcept;
		int GetExitCode(void) const;
		bool IsRunning(void) const noexcept;
	protected:
		virtual bool VCheckResources(void);
		virtual bool VPreloadResources(void);
		virtual bool VInitRmlUi(void);
		virtual void VShutdownRmlUi(void);
		virtual void VRegisterGameEvents(void);
		virtual bool VAttachAsClient(void);
		virtual void VCreateNetworkEventForwarder(void);
		virtual void VDestroyNetworkEventForwarder(void);
	private:
		bool LoadConfig(const std::filesystem::path &kConfigPath);
		void RegisterEngineEvents(void);
		void UpdateFPS(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_ENGINEAPP_HPP_ */
