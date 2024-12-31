#include "Engine/EngineStd.hpp"
#include "Scripting/ScriptResource.hpp"

#include "Engine/EngineApp.hpp"

namespace BGE
{
	std::string ScriptResourceLoader::VGetPattern(void) const
	{
		return "*.lua";
	}

	ResourceType ScriptResourceLoader::VGetType(void) const
	{
		return ResourceType::kScript;
	}

	bool ScriptResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool ScriptResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool ScriptResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t ScriptResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool ScriptResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		auto &gameLogic = GetEngineApp().GetGameLogic(); // Retrieve GameLogic
		auto &lua = gameLogic.GetLuaScriptManager(); // Retrieve global LuaScriptManager

		// Execute the script in the global Lua manager
		if (gameLogic.CanRunScripts())
		{
			lua.VExecuteString(pRawBuffer);
		}
		return true;
	}
} // End namespace (BGE)
