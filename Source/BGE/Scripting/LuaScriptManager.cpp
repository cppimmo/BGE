#include "Engine/EngineStd.hpp"
#include "Scripting/LuaScriptManager.hpp"

BGE::LuaScriptManager::LuaScriptManager(void)
{
}

bool BGE::LuaScriptManager::VInit(void)
{
	// Open necessary libraries
	constexpr std::array<sol::lib, 2> kLIBS = {sol::lib::base, sol::lib::table};
	std::apply([&](auto &&...args) mutable { m_state.open_libraries(args...); }, kLIBS);

	m_state.set_panic(LuaScriptManager::OnPanic); // Set the panic handler
	m_state.set_exception_handler(LuaScriptManager::OnHandleException);
	return true;
}

void BGE::LuaScriptManager::VExecuteString(std::string_view str)
{
	m_state.do_string(str);
}

void BGE::LuaScriptManager::VExecuteFile(std::string_view filename)
{
	m_state.do_file(std::string(filename));
}

void BGE::LuaScriptManager::CollectGarbage(void)
{
	m_state.collect_garbage();
}

sol::state &BGE::LuaScriptManager::GetState(void) noexcept
{
	return m_state;
}

const sol::state &BGE::LuaScriptManager::GetState(void) const noexcept
{
	return m_state;
}

std::size_t BGE::LuaScriptManager::GetMemoryUsed(void) const
{
	return m_state.memory_used();
}

int BGE::LuaScriptManager::OnHandleException(lua_State* pLuaState, sol::optional<const std::exception &>, std::string_view what)
{
	std::ostringstream oss;
	oss << "[sol2] An exception occurred: ";
	oss.write(what.data(), static_cast<std::streamsize>(what.size()));
	oss << std::endl;
	lua_pushlstring(pLuaState, what.data(), what.size());
	return 1;
}


int BGE::LuaScriptManager::OnPanic(lua_State* pLuaState)
{
	// Retrieve the error message at the top of the stack
	const char *pErrorMessage = lua_tostring(pLuaState, -1);
	if (!pErrorMessage)
	{
		pErrorMessage = "Unknown error (no error message provided by Lua).";
	}

	// Start forming up the log message
	std::ostringstream oss;
	oss << "Lua PANIC: " << pErrorMessage << '\n';

	// Log the Lua stack state, if possible
	oss << "Dumping Lua stack state:\n";
	int top = lua_gettop(pLuaState); // Get the top of the stack
	for (int i = 1; i <= top; ++i)
	{
		const int kType = lua_type(pLuaState, i);
		switch (kType)
		{
		case LUA_TNIL:
			oss << i << ": (nil)\n";
			break;
		case LUA_TNUMBER:
			oss << i << ": " << lua_tonumber(pLuaState, i) << " (number)\n";
			break;
		case LUA_TBOOLEAN:
			oss << i << ": " << (lua_toboolean(pLuaState, i) ? "true" : "false") << " (boolean)\n";
			break;
		case LUA_TSTRING:
			oss << i << ": " << lua_tostring(pLuaState, i) << " (string)\n";
			break;
		case LUA_TTABLE:
			oss << i << ": (table)\n";
			break;
		case LUA_TFUNCTION:
			oss << i << ": " << " (function)\n";
			break;
		case LUA_TUSERDATA:
			oss << i << ": (userdata)\n";
			break;
		case LUA_TTHREAD:
			oss << i << ": (thread)\n";
			break;
		case LUA_TLIGHTUSERDATA:
			oss << i << ": (lightuserdata)\n";
			break;
		default:
			oss << i << ": " << lua_typename(pLuaState, i) << '\n';
			break;
		}
	}
	// Optionally capture additional metadata about the state
	oss << "Lua state information:\n";
	oss << "Number of elements in the stack: " << top << '\n';

	constexpr std::string_view kERROR_MSG = "Lua PANIC handler terminating application due to unresolvable error!";
	// Cleanup
	oss << kERROR_MSG << '\n';

	BGE_WARNING("%s", oss.str().c_str());
	BGE_ERROR("%s", kERROR_MSG.data());
	oss.flush();

	return 0;
}
