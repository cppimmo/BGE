#include <Engine/EngineStd.hpp>
#include "TestGame.hpp"

using namespace BGE;

int main(int numArgs, char *pArgs[])
{
	// Create the application layer:
	if (!CreateEngineApp<TestGameApp>())
		return BGE_EXIT_FAILURE;

	return EngineMain(numArgs, pArgs);
}

// Start of Tank Battles application layer implementation:
std::string TestGameApp::VGetGameTitle(void)
{
	return "Tank Battles";
}

std::string TestGameApp::VGetGameAppDirectory(void)
{
	auto saveGameDir = GetSaveGameDirectory("cppimmo", SnakeCaseString(VGetGameTitle()));
	if (!saveGameDir)
	{
		auto currDirectory = GetCurrentDirectory();
		BGE_ASSERT(currDirectory.has_value());
		return *currDirectory;
	}
	return *saveGameDir;
}

std::string TestGameApp::VGetIcon(void)
{
	return "TankBattles.ico";
}

void TestGameApp::VRegisterGameEvents(void)
{
}

void TestGameApp::VCreateNetworkEventForwarder(void)
{
}

void TestGameApp::VDestroyNetworkEventForwarder(void)
{
}

class TestProcess : public Process
{
	Timer m_timer;
public:
	virtual bool VOnInit(void) override
	{
		const bool kbResult = Process::VOnInit();
		BGE_INFO("Process(%d) created", GetID());
		m_timer.Start();
		return kbResult;
	}
	virtual void VOnUpdate(float deltaTime) override
	{
		if (m_timer.GetElapsedSecs() >= 3.0)
		{
			BGE_INFO("Process(%d) time elapsed", GetID());
			m_timer.Reset();
		}
	}
	virtual void VOnSuccess(void) override
	{
		BGE_INFO("Process(%d) successed", GetID());
	}
	virtual void VOnAbort(void) override
	{
		BGE_INFO("Process(%d) aborted", GetID());
	}
};

UniqueBaseGameLogicPtr TestGameApp::VCreateGameAndView(void)
{
	auto pGameLogic = std::make_unique<TestGameLogic>();
	if (!pGameLogic->Init())
	{
		BGE_ERROR("Failure initializing game logic");
		return nullptr;
	}
	// TODO: Remove testing code.
	StrongProcessPtr pTestProcess = std::make_shared<TestProcess>();
	pGameLogic->GetProcessManager().AttachProcess(pTestProcess);

	return pGameLogic;
}

int TestGameApp::VGetRendererImpl(void)
{
	return 0;
}

// Start of TestGameLogic implementation:
TestGameLogic::TestGameLogic(void)
{
}

TestGameLogic::~TestGameLogic(void)
{
}

void TestGameLogic::VChangeState(BGE::BaseGameState state)
{
	BaseGameLogic::VChangeState(state);
}
