/*******************************************************************************
 * @file   BaseGameLogic.cpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Definition of IGameLogic interface & BaseGameLogic subclass.
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
#include "Engine/BaseGameLogic.hpp"

#include "Scripting/ScriptExports.hpp"

BGE::BaseGameLogic::BaseGameLogic(void)
    : m_state(BaseGameState::kInitializing),
      m_bProxy(false)
{
    m_pProcessManager = std::make_unique<ProcessManager>();
    m_pLuaScriptManager = std::make_unique<LuaScriptManager>();
}

BGE::BaseGameLogic::~BaseGameLogic(void)
{
    VDeregisterDelegates();
}

bool BGE::BaseGameLogic::VInit(void)
{
    VRegisterDelegates();

    // Init Lua script manager
    if (!m_pLuaScriptManager->VInit())
    {
        BGE_ERROR("Could not initialize Lua state");
        return false;
    }
    return true;
}

bool BGE::BaseGameLogic::VPostInit(void)
{
    return true;
}

void BGE::BaseGameLogic::VAddView(StrongIGameViewPtr pView, ActorID aID)
{
    BGE_ASSERT(pView);

    // This makes sure that all views have a non-zero view id
	auto viewID = static_cast<GameViewID>(m_gameViews.size());

    // Add the game view to the list
	m_gameViews.push_back(pView);

    // Attempt to initialize the game view, if it has not already been done
    if (!pView->VIsInitialized())
    {
        BGE_ERROR_IF(pView->VInit(), "Failed to initialize game view");
    }

	pView->VOnAttach(viewID, aID); // Call the attach routine
	pView->VOnRestore(); // Call the store routine
}

void BGE::BaseGameLogic::VRemoveView(StrongIGameViewPtr pView)
{
    m_gameViews.remove(pView);
}

BGE::WeakActorPtr BGE::BaseGameLogic::VGetActor(ActorID ID)
{
    return WeakActorPtr{};
}

void BGE::BaseGameLogic::VDestroyActor(ActorID ID)
{
}

bool BGE::BaseGameLogic::VLoadGame(std::string_view levelResource)
{
    return true;
}

void BGE::BaseGameLogic::VSetProxy(void)
{
}

void BGE::BaseGameLogic::VOnUpdate(float deltaTime, float elapsedTime)
{
    //int deltaMS = static_cast<int>(elapsedTime * 1'000.0f);
    m_lifetimeTimer.Start();

    using enum BaseGameState;
    switch (m_state)
    {
    case kInitializing:
        // TODO: Fix this.
        VChangeState(BaseGameState::kRunning);
        break;
    case kMainMenu:
        break;
    case kLoadingGameEnvironment:
        break;
    case kWaitingForPlayersToLoadEnvironment:
        break;
    case kSpawningPlayersActors:
        VChangeState(kRunning);
        break;
    case kWaitingForPlayers:
        break;
    case kRunning:
        m_pProcessManager->UpdateProcesses(deltaTime);
        break;
    default:
        BGE_ERROR("Unrecognized game state.");
    }

    // Update all game views
    for (auto &pView : m_gameViews)
    {
        pView->VOnUpdate(deltaTime);
    }

    m_lifetimeTimer.Stop();
}

void BGE::BaseGameLogic::VChangeState(BaseGameState state)
{
    // TODO: Add extra logic.
    m_state = state;
}

BGE::ProcessManager &BGE::BaseGameLogic::GetProcessManager(void) noexcept
{
    BGE_ASSERT(m_pProcessManager);
    return *m_pProcessManager.get();
}

BGE::LuaScriptManager &BGE::BaseGameLogic::GetLuaScriptManager(void) noexcept
{
    BGE_ASSERT(m_pLuaScriptManager);
    return *m_pLuaScriptManager.get();
}

BGE::GameViewList &BGE::BaseGameLogic::GetGameViews(void) noexcept
{
    return m_gameViews;
}

bool BGE::BaseGameLogic::IsProxy(void) const
{
    return m_bProxy;
}

void BGE::BaseGameLogic::SetProxy(bool bProxy) noexcept
{
    m_bProxy = bProxy;
}

bool BGE::BaseGameLogic::CanRunScripts(void) const
{
    return !IsProxy() && GetState() != BaseGameState::kRunning;
}

BGE::BaseGameState BGE::BaseGameLogic::GetState() const
{
    return m_state;
}

void BGE::BaseGameLogic::VRegisterDelegates(void)
{
}

void BGE::BaseGameLogic::VDeregisterDelegates(void)
{
}
