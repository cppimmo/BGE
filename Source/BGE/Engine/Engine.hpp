/*=============================================================================*
 * Engine.hpp - Definition of the application layer.
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
#ifndef _BGE_ENGINE_HPP_
#define _BGE_ENGINE_HPP_

#include "MainLoop/Initialization.hpp"
#include "Utilities/Types.hpp"
#include "Engine/BaseGameLogic.hpp"
#include "Events/EventManager.hpp"
#include "Resources/Localizer.hpp"

#include <map>
#include <string>

namespace BGE
{
	// Set global application pointer to the instance of an app layer class.
	extern UniqueEngineAppPtr g_pApp; // Global application layer pointer (use as singleton)
	
	//! Return reference to the global EngineApp instance.
	EngineApp &GetEngineApp(void);
	// Call in game programs to create an instance of the EngineApp.
	template <typename DerivedApp, typename... Args>
	bool CreateEngineApp(Args... args) requires(std::derived_from<DerivedApp, EngineApp>)
	{
		g_pApp = std::make_unique<DerivedApp>(args...);
		return g_pApp.operator bool();
	}
	/**
	 * Application layer class.
	 */
	class EngineApp : public INonCopyable, public INonMovable
	{
	protected:
		bool m_bRunning; // True if game is in the main loop
		bool m_bQuitRequested; // True if the exit sequence is nigh
		bool m_bQuitting; // True if the exit sequence is being ran
		bool m_bHasQuit; // true if the exit sequence has been run
		bool m_bEditorRunning; // True if the game editor is running
		bool m_bResourceCheck;
		TextStringMap m_textStrings; // Localized string container
		UniqueLocalizerPtr m_pLocalizer; //!< Localization handler
		UniqueEventManagerPtr m_pEventManager; //!< Main event manager
		UniqueBaseGameLogicPtr m_pGameLogic;
		// TODO: Add event manager.
	public:
		EngineApp(void);
		virtual ~EngineApp(void);
		// EngineApp interface:
		virtual bool VInitInstance(void);
		virtual UniqueBaseGameLogicPtr VCreateGameAndView(void) = 0;
		virtual bool VLoadGame(void);
		virtual std::string VGetGameTitle(void) = 0; // Application related data
		virtual std::string VGetGameAppDirectory(void) = 0;
		virtual std::string VGetIcon(void) = 0; // Icon filename
		virtual int VGetRendererImpl(void) = 0; // Type of renderer

		// These are marked static so it will be easier to pass them as arguments:
		static void OnUpdate(float deltaTime, float elsapsedTime);
		static void OnRender(void);
		static bool OnHandleEvent(const SDL_Event &kEvent);
		static void OnDisplayChange(int colorDepth, int width, int height);
		void OnShutdown(void);
		// Accessors:
		Localizer &GetLocalizer(void);
		EventManager &GetEventManager(void);
		BaseGameLogic &GetGameLogic(void);
		int GetExitCode(void) const;
	protected:
		virtual void VRegisterGameEvents(void);
		virtual void VCreateNetworkEventForwarder(void);
		virtual void VDestroyNetworkEventForwarder(void);
	private:
		void RegisterEngineEvents(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_ENGINE_HPP_ */
