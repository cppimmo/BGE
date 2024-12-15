#ifndef _BGE_LUASCRIPTMANAGER_HPP_
#define _BGE_LUASCRIPTMANAGER_HPP_

#include "Scripting/ScriptManager.hpp"

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
		virtual void VExecuteFile(std::string_view filename) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_LUASCRIPTMANAGER_HPP_ */
