#ifndef _BGE_LUASCRIPTMANAGER_HPP_
#define _BGE_LUASCRIPTMANAGER_HPP_

#include "Scripting/ScriptManager.hpp"

#include <lua.hpp>
#include <sol/sol.hpp>

namespace BGE
{
	class IScriptManager; // Forward declare
	class LuaScriptManager; // Forward declare
	BGE_DECLARE_PTR(LuaScriptManager);

	class LuaScriptManager : public IScriptManager
	{
		sol::state m_state;
	public:
		LuaScriptManager(void);
		virtual ~LuaScriptManager(void) = default;
		// IScriptManager's interface:
		virtual bool VInit(void) override;
		virtual void VExecuteString(std::string_view str) override;
		virtual void VExecuteFile(std::string_view fileName) override;
		// Interface:
		//! Attempt to run the Lua state's garbage collector.
		void CollectGarbage(void);
		// Accessors:
		sol::state &GetState(void) noexcept;
		const sol::state &GetState(void) const noexcept;
		//! Return the number of bytes used by the Lua state in memory.
		std::size_t GetMemoryUsed(void) const;
	private:
		void HandleProtectedFunctionResult(const sol::protected_function_result &kResult);
		static int OnHandleException(lua_State *pLuaState, sol::optional<const std::exception &>, std::string_view what);
		static int OnPanic(lua_State *pLuaState);
	};
} // End namespace (BGE)

#endif /* !_BGE_LUASCRIPTMANAGER_HPP_ */
