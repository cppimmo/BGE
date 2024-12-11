#include "TestGameStd.hpp"
#include "TestProcesses.hpp"

#include "TestEvents.hpp"

using namespace TestGame;

bool TestProcess::VOnInit(void)
{
	const bool kbResult = Process::VOnInit();
	BGE_INFO("Process(%d) created", GetID());
	m_timer.Start();
	return kbResult;
}

void TestProcess::VOnUpdate(float deltaTime)
{
	if (m_timer.GetElapsedSecs() >= 3.0)
	{
		BGE_INFO("Process(%d) time elapsed", GetID());
		m_timer.Reset();
		// Create & queue a test event
		auto pTestEvent = std::make_shared<TestGame::EventData_Test>();
		BGE_QUEUE_GEVENT(pTestEvent);
	}
}

void TestProcess::VOnSuccess(void)
{
	BGE_INFO("Process(%d) successed", GetID());
}

void TestProcess::VOnAbort(void)
{
	BGE_INFO("Process(%d) aborted", GetID());
}
