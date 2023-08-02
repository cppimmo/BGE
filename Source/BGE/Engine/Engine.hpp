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

#include <map>
#include <string>

namespace BGE
{
	// Set global application pointer to the instance of an app layer class.
	extern UniqueEngineAppPtr g_pApp; // Global application layer pointer (use as singleton)
	
	UniqueEngineAppPtr &GetEngineAppPtr(void);
	// Call in game programs to create an instance of the EngineApp.
	template <typename DerivedApp, typename... Args>
	bool CreateEngineApp(Args... args) requires(std::derived_from<DerivedApp, EngineApp>)
	{
		GetEngineAppPtr() = std::make_unique<DerivedApp>(args...);
		return GetEngineAppPtr().operator bool();
	}
	/**
	 * Application layer class.
	 */
	class EngineApp : public INonCopyable, public INonMoveable
	{
	protected:
		bool m_isRunning; // True if game is in the main loop
		bool m_quitRequested; // True if the exit sequence is nigh
		bool m_quitting; // True if the exit sequence is being ran
		bool m_hasQuit; // true if the exit sequence has been run
		bool m_isEditorRunning; // True if the game editor is running
		TextStringMap m_textStrings; // Localized string container
		UniqueBaseGameLogicPtr m_pGame;
	public:
		EngineApp(void);
		virtual ~EngineApp(void);
	
		virtual bool VInitInstance(void);
		virtual BaseGameLogic *VCreateGameAndView(void) = 0;
		virtual bool VLoadGame(void);
		virtual std::string VGetGameTitle(void) = 0; // Application related data
		virtual std::string VGetGameAppDirectory(void) = 0;
		virtual std::string VGetIcon(void) = 0; // Icon filename
		virtual int VGetRendererImpl(void) = 0; // Type of renderer

		bool LoadStrings(std::string_view language);
		std::wstring GetString(std::wstring_view sID);
		// These are marked static so it will be easier to pass them as arguments:
		static void OnUpdate(float deltaTime, float elsapsedTime);
		static void OnRender(void);
		static void OnHandleEvent(const SDL_Event &event);
		static void OnDisplayChange(int colorDepth, int width, int height);
		void OnClose(void);
		int GetExitCode(void) { return BGUTGetExitCode(); }
	protected:
		virtual void VRegisterGameEvents(void);
		virtual void VCreateNetworkEventForwarder(void);
		virtual void VDestroyNetworkEventForwarder(void);
	private:
		void RegisterEngineEvents(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_ENGINE_HPP_ */