#include "TestGameStd.hpp"
#include "TestGame.hpp"

#include <Events/EventRegistry.hpp>

#include "TestEvents.hpp"
#include "TestProcesses.hpp"
#include "TestGameView.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

/**
 * NOTE: The main function should only serve to create the engine app & call the
 * engine main function.
 */
int main(int numArgs, char *pArgs[])
{
	// Create the application layer:
	if (!BGE::CreateEngineApp<TestGame::TestGameApp>())
	{
		return BGE::kBGE_EXIT_FAILURE;
	}

	return BGE::EngineMain(numArgs, pArgs);
}

namespace TestGame
{
	// Start of Test Game application layer implementation:
	std::string TestGameApp::VGetGameTitle(void)
	{
		return "Test Game";
	}

	std::string TestGameApp::VGetGameAppDirectory(void)
	{
		auto saveGameDir = BGE::GetSaveGameDirectory("cppimmo", BGE::SnakeCaseString(VGetGameTitle()));
		if (!saveGameDir)
		{
			auto currDirectory = BGE::GetCurrentDirectory();
			BGE_ASSERT(currDirectory.has_value());
			return *currDirectory;
		}
		return *saveGameDir;
	}

	std::string TestGameApp::VGetIcon(void)
	{
		return "game_icon.bmp";
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

	BGE::UniqueBaseGameLogicPtr TestGameApp::VCreateGameAndView(void)
	{
		auto pGameLogic = std::make_unique<TestGameLogic>();
		if (!pGameLogic->VInit())
		{
			BGE_ERROR("Failure initializing game logic");
			return nullptr;
		}
		// TODO: Remove testing code.
		BGE::StrongProcessPtr pTestProcess = std::make_shared<TestProcess>();
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
		auto pHumanView = std::make_shared<TestGameView>();
		VAddView(pHumanView);
	}

	TestGameLogic::~TestGameLogic(void)
	{
	}

	bool TestGameLogic::VInit(void)
	{
		return BaseGameLogic::VInit();
	}

	bool TestGameLogic::VPostInit(void)
	{
		bool bResult = BaseGameLogic::VPostInit();

		auto &app = BGE::GetEngineApp();
		auto &resCache = app.GetResourceCache();

		BGE_LOG("TestGame", "What's going on");
		auto pScriptHandle = resCache.GetHandle(BGE::Resource("Assets\\Scripts\\Test.lua"));

		BGE_LOG("TestGame", "Human views: %u", app.GetHumanViewCount());

		auto pHumanView = app.GetHumanView(0);
		if (pHumanView)
		{
			BGE_LOG("TestGame", "There is a human view at index: %u", 0u);
		}

		//m_pLuaScriptManager->VExecuteFile("Assets/Scripts/Test.lua");

		return bResult;
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
} // End namespace (TestGame)
