#include "Engine/EngineStd.hpp"
#include "HumanView.hpp"

BGE::HumanView::HumanView(void)
{
}

BGE::HumanView::~HumanView(void)
{
}

void BGE::HumanView::VOnRestore(void)
{
}

void BGE::HumanView::VOnRender(float deltaTime, float elapsedTime)
{
}

void BGE::HumanView::VOnLostDevice(void)
{
}

BGE::GameViewType BGE::HumanView::VGetType(void)
{
}

BGE::GameViewID BGE::HumanView::VGetID(void)
{
}

void BGE::HumanView::VOnAttach(GameViewID vID, ActorID aID)
{
}

bool BGE::HumanView::VOnHandleEvent(const SDL_Event &kEvent)
{
	return false;
}

void BGE::HumanView::VOnUpdate(float deltaTime)
{
}

BGE::ProcessManager &BGE::HumanView::GetProcessManager(void)
{
	BGE_ASSERT(m_pProcessManager);
	return *m_pProcessManager.get();
}

void BGE::HumanView::PlaySoundDelegate(StrongIEventDataPtr pEventData)
{
}

void BGE::HumanView::GameStateDelegate(StrongIEventDataPtr pEventData)
{
}

void BGE::HumanView::VRegisterDelegates(void)
{
}

void BGE::HumanView::VDeregisterDelegates(void)
{
}
