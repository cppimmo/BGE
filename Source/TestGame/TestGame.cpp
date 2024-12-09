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

class EventData_Test; // Forware declare

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

			auto &eventManager = GetEngineApp().GetEventManager();
			// Create & queue a test event
			auto pTestEvent = std::make_shared<EventData_Test>();
			eventManager.VQueueEvent(std::static_pointer_cast<IEventData>(pTestEvent));
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

class EventData_Test : public BaseEventData
{
public:
	static constexpr EventType kEVENT_TYPE = 0xDEADBEEF;
public:
	EventData_Test(void) = default;

	virtual const EventType &VGetEventType(void) const override
	{
		return kEVENT_TYPE;
	}

	virtual StrongIEventDataPtr VCopy(void) const override
	{
		return nullptr; //std::make_shared<EventData_Test>(*this);
	}

	virtual void VSerialize(std::ostringstream &oss) const override
	{
	}

	virtual void VDeserialize(std::istringstream &iss) override
	{
	}

	virtual constexpr std::string_view VGetName(void) const override
	{
		return "EventData_Test";
	}
};

void TestGameLogic::VRegisterDelegates(void)
{
	auto &eventManager = GetEngineApp().GetEventManager();
	eventManager.VAddListener(fastdelegate::MakeDelegate(this, &TestGameLogic::TestDelegate), EventData_Test::kEVENT_TYPE);
}

void TestGameLogic::VDeregisterDelegates(void)
{
	auto &eventManager = GetEngineApp().GetEventManager();
	eventManager.VRemoveListener(fastdelegate::MakeDelegate(this, &TestGameLogic::TestDelegate), EventData_Test::kEVENT_TYPE);
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
