#include "Engine/EngineStd.hpp"
#include "Engine/BaseGameLogic.hpp"

BGE::BaseGameLogic::BaseGameLogic(void)
    : m_state(BaseGameState::kInitializing),
      m_bProxy(false)
{
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

void BGE::BaseGameLogic::VAddView(void)
{
}

void BGE::BaseGameLogic::VRemoveView(void)
{
}

BGE::WeakActorPtr BGE::BaseGameLogic::VGetActor(ActorID ID)
{
}

void BGE::BaseGameLogic::VDestroyActor(ActorID ID)
{
}

bool BGE::BaseGameLogic::VLoadGame(std::string_view levelResource)
{
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
        m_processManager.UpdateProcesses(deltaMS);
        break;
    default:
        BGE_ERROR("Unrecognized game state.");
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
    return m_processManager;
}

const BGE::ProcessManager &BGE::BaseGameLogic::GetProcessManager(void) const noexcept
{
    return m_processManager;
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
