#include "Engine/EngineStd.hpp"
#include "Scripting/ScriptResource.hpp"

#include "Engine/EngineApp.hpp"

const std::string &BGE::ScriptResourceLoader::VGetPattern(void) const
{
	return m_pattern;
}

bool BGE::ScriptResourceLoader::VUseRawFile(void) const
{
	return false;
}

bool BGE::ScriptResourceLoader::VDiscardRawBufferAfterLoad(void)
{
	return true;
}

bool BGE::ScriptResourceLoader::VAddNullZero(void)
{
	return true;
}

std::size_t BGE::ScriptResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
{
	return rawSize;
}

bool BGE::ScriptResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
{
	if (size <= 0)
		return false;

	auto &gameLogic = GetEngineApp().GetGameLogic(); // Retrieve GameLogic
	auto &lua = gameLogic.GetLuaScriptManager(); // Retrieve global LuaScriptManager

	// Execute the script in the global Lua manager
	if (gameLogic.CanRunScripts())
	{
		lua.VExecuteString(pRawBuffer);
	}
	return true;
}
