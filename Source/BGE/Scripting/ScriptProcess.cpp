#include "Engine/EngineStd.hpp"
#include "Scripting/ScriptProcess.hpp"

namespace
{
	inline sol::state &GetLuaState(void)
	{
		auto &app = BGE::GetEngineApp();
		return app.GetGameLogic().GetLuaScriptManager().GetState();
	}
} // End namespace

BGE::ScriptProcess::ScriptProcess(void)
	: m_frequency(0), m_time(0)
{
	auto &state = GetLuaState();
}

bool BGE::ScriptProcess::VOnInit(void)
{
	bool bResult = Process::VOnInit();
	// Try to call the script init function
	if (m_initFunc)
	{
		bResult = bResult && m_initFunc.call<bool>();
	}
	/*
	 * Fail immediately if no update function is declared. This indicates an error in the script
	 * side of the declaration.
	 */
	if (!m_updateFunc)
	{
		Fail();
	}
	return bResult;
}

void BGE::ScriptProcess::VOnUpdate(float deltaTime)
{
	m_time += deltaTime;
	if (m_time >= m_frequency)
	{
		m_updateFunc(); // Call the update function
		m_time = 0; // Reset the elapsed time
	}
}

void BGE::ScriptProcess::VOnSuccess(void)
{
	if (m_successFunc)
	{
		m_successFunc();
	}
}

void BGE::ScriptProcess::VOnFail(void)
{
	if (m_failFunc)
	{
		m_failFunc();
	}
}

void BGE::ScriptProcess::VOnAbort(void)
{
	if (m_abortFunc)
	{
		m_abortFunc();
	}
}
