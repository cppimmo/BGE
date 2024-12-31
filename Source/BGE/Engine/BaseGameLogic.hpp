/*******************************************************************************
 * @file   BaseGameLogic.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Declaration of IGameLogic interface & BaseGameLogic subclass.
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
#ifndef _BGE_BASEGAMELOGIC_HPP_
#define _BGE_BASEGAMELOGIC_HPP_

#include "Actors/Actor.hpp"
#include "Actors/ActorFactory.hpp"
#include "MainLoop/ProcessManager.hpp"
#include "UI/GameView.hpp"
#include "Utilities/Timer.hpp"
#include "Scripting/LuaScriptManager.hpp"

namespace BGE
{
	enum struct BaseGameState
	{
		kInvalid,
		kInitializing,
		kMainMenu,
		kWaitingForPlayers,
		kLoadingGameEnvironment,
		kWaitingForPlayersToLoadEnvironment,
		kSpawningPlayersActors,
		kRunning
	};

	class IGameLogic
	{
	public:
		virtual ~IGameLogic(void) = default;

		virtual bool VInit(void) = 0;
		virtual bool VPostInit(void) = 0;
		virtual WeakActorPtr VGetActor(ActorID ID) = 0;
		//virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4 *initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID)=0;
		virtual void VDestroyActor(ActorID ID) = 0;
		virtual bool VLoadGame(std::string_view levelResource) = 0;
		virtual void VSetProxy(void) = 0;
		virtual void VOnUpdate(float time, float elapsedTime) = 0;
		virtual void VChangeState(BaseGameState state) = 0;
		//virtual void VMoveActor(const ActorId id, Mat4x4 const &mat)=0;
		//virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;
	};

	class BaseGameLogic : public IGameLogic
	{
		friend class EngineApp;
	protected:
		BaseGameState m_state;
		bool m_bProxy;
		Timer m_lifetimeTimer;
		UniqueProcessManagerPtr m_pProcessManager;
		UniqueLuaScriptManagerPtr m_pLuaScriptManager;
		ActorFactory m_actorFactory;
		GameViewList m_gameViews;
	public:
		BaseGameLogic(void);
		virtual ~BaseGameLogic(void);

		// BaseGameLogic interface:
		virtual void VAddView(StrongIGameViewPtr pView, ActorID aID = kINVALID_ACTOR_ID);
		virtual void VRemoveView(StrongIGameViewPtr pView);
		// IGameLogic interface:
		virtual bool VInit(void) override;
		virtual bool VPostInit(void) override;
		virtual WeakActorPtr VGetActor(ActorID ID) override;
		virtual void VDestroyActor(ActorID ID) override;
		virtual bool VLoadGame(std::string_view levelResource) override;
		virtual void VSetProxy(void) override;
		virtual void VOnUpdate(float time, float elapsedTime) override;
		virtual void VChangeState(BaseGameState state) override;
		// Accessors:
		ProcessManager &GetProcessManager(void) noexcept;
		const ProcessManager &GetProcessManager(void) const noexcept;
		LuaScriptManager &GetLuaScriptManager(void) noexcept;
		const LuaScriptManager &GetLuaScriptManager(void) const noexcept;
		GameViewList &GetGameViews(void) noexcept;
		const GameViewList &GetGameViews(void) const noexcept;
		bool IsProxy(void) const;
		void SetProxy(bool bProxy) noexcept;
		bool CanRunScripts(void) const;
		BaseGameState GetState(void) const;
	protected:
		virtual void VRegisterDelegates(void);
		virtual void VDeregisterDelegates(void);
	private:
	};
} // End namespace (BGE)

#endif /* !_BGE_BASEGAMELOGIC_HPP_ */
