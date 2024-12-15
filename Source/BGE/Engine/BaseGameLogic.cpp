#include "Engine/EngineStd.hpp"
#include "Engine/BaseGameLogic.hpp"

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

bool BGE::BaseGameLogic::Init(void)
{
    VRegisterDelegates();
    // TODO: Perform other logic here
    VChangeState(BaseGameState::kRunning);
    return true;
}

void BGE::BaseGameLogic::VAddView(StrongIGameViewPtr pView, ActorID aID)
{
    // This makes sure that all views have a non-zero view id
	auto viewID = static_cast<GameViewID>(m_gameViews.size());
	m_gameViews.push_back(pView);
	pView->VOnAttach(viewID, aID);
	pView->VOnRestore();
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

void BGE::BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
    int deltaMS = static_cast<int>(elapsedTime * 1'000.0f);
    m_lifetimeTimer.Start();

    using enum BaseGameState;
    switch (m_state)
    {
    case kInitializing:
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
        m_pProcessManager->UpdateProcesses(deltaMS);
        break;
    default:
        BGE_ERROR("Unrecognized game state.");
    }

    // Update all game views
    for (auto &view : m_gameViews)
    {
        view->VOnUpdate(deltaMS);
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

const BGE::ProcessManager &BGE::BaseGameLogic::GetProcessManager(void) const noexcept
{
    BGE_ASSERT(m_pProcessManager);
    return *m_pProcessManager.get();
}

BGE::LuaScriptManager &BGE::BaseGameLogic::GetLuaScriptManager(void) noexcept
{
    BGE_ASSERT(m_pLuaScriptManager);
    return *m_pLuaScriptManager.get();
}

const BGE::LuaScriptManager &BGE::BaseGameLogic::GetLuaScriptManager(void) const noexcept
{
    BGE_ASSERT(m_pLuaScriptManager);
    return *m_pLuaScriptManager.get();
}

BGE::GameViewList &BGE::BaseGameLogic::GetGameViews(void) noexcept
{
    return m_gameViews;
}

const BGE::GameViewList &BGE::BaseGameLogic::GetGameViews(void) const noexcept
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

void BGE::BaseGameLogic::VRegisterDelegates(void)
{
}

void BGE::BaseGameLogic::VDeregisterDelegates(void)
{
}
