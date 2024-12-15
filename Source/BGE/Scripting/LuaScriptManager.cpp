#include "Engine/EngineStd.hpp"
#include "Scripting/LuaScriptManager.hpp"

BGE::LuaScriptManager::LuaScriptManager(void)
{
}

bool BGE::LuaScriptManager::VInit(void)
{
	return true;
}

void BGE::LuaScriptManager::VExecuteString(std::string_view str)
{
}

void BGE::LuaScriptManager::VExecuteFile(std::string_view filename)
{
}
