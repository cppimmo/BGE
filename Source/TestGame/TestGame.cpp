#include "TestGameStd.hpp"
#include "TestGame.hpp"

#include <Events/EventRegistry.hpp>

#include "TestEvents.hpp"
#include "TestProcesses.hpp"

using namespace BGE;
using namespace TestGame;

int main(int numArgs, char *pArgs[])
{
	// Create the application layer:
	if (!CreateEngineApp<TestGameApp>())
		return BGE_EXIT_FAILURE;

	return EngineMain(numArgs, pArgs);
}

class EventData_Test; // Forware declare

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
	BGE_REGISTER_EVENT(EventData_Test);
}

void TestGameApp::VCreateNetworkEventForwarder(void)
{
}

void TestGameApp::VDestroyNetworkEventForwarder(void)
{
}

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

void TestGameLogic::VRegisterDelegates(void)
{
	BGE_ADD_GEVENT_LISTENER(this, &TestGameLogic::TestDelegate, EventData_Test);
}

void TestGameLogic::VDeregisterDelegates(void)
{
	BGE_REMOVE_GEVENT_LISTENER(this, &TestGameLogic::TestDelegate, EventData_Test);
}

void TestGameLogic::TestDelegate(BGE::StrongIEventDataPtr pEventData)
{
	// Attempt to cast to EventData_Test
	auto pEventDataTest = std::dynamic_pointer_cast<EventData_Test>(pEventData);
	// Ensure valid pointer
	if (!pEventDataTest)
	{
		BGE_ERROR("Could not cast to EventData_Test");
		return;
	}

	BGE_LOG("TestDelegate", "Called listener for %s", pEventDataTest->VGetName().data());
}
