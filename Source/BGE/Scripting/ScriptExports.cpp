#include "Engine/EngineStd.hpp"
#include "Scripting/ScriptExports.hpp"

#include "Engine/EngineApp.hpp"
#include "UI/DebugConsole.hpp"
#include "Debugging/Logger.hpp"

namespace BGE::ScriptExports
{
	void Register(void)
	{
		auto &app = GetEngineApp();
		auto &lua = app.GetGameLogic().GetLuaScriptManager();
		sol::state &luaState = lua.GetState();

		// Create the Console module/table
		sol::table consoleModule = luaState.create_table();

		// Add functions to the Console table
		consoleModule.set_function("Log", InternalScriptExports::ConsoleLog);
		consoleModule.set_function("Open", InternalScriptExports::ConsoleOpen);
		consoleModule.set_function("Close", InternalScriptExports::ConsoleClose);

		// Assign the table to a global variable/module in Lua
		luaState["Console"] = consoleModule;

		// Create the Logger module/table
		sol::table loggerModule = luaState.create_table();

		// Add the Logger's levels to the table
		loggerModule["Level"] = luaState.create_table_with(
			"Fatal", static_cast<int>(Logger::Level::Fatal),
			"Error", static_cast<int>(Logger::Level::Error),
			"Warning", static_cast<int>(Logger::Level::Warning),
			"Info", static_cast<int>(Logger::Level::Info),
			"Log", static_cast<int>(Logger::Level::Log)
		);

		// Add functions to the Logger table
		loggerModule.set_function("Fatal", InternalScriptExports::LoggerFatal);
		loggerModule.set_function("FatalIf", InternalScriptExports::LoggerFatalIf);
		loggerModule.set_function("Error", InternalScriptExports::LoggerError);
		loggerModule.set_function("ErrorIf", InternalScriptExports::LoggerErrorIf);
		loggerModule.set_function("Warning", InternalScriptExports::LoggerWarning);
		loggerModule.set_function("Info", InternalScriptExports::LoggerInfo);
		loggerModule.set_function("Log", InternalScriptExports::LoggerLog);
		loggerModule.set_function("LevelToString", InternalScriptExports::LoggerLevelToString);

		// Assign the table to a global variable/module in Lua
		luaState["Logger"] = loggerModule;

		// TODO: This is for testing; replace it later.
		/*luaState.set_function("LoadResource", [](sol::string_view resource)
		{
			auto &app = GetEngineApp();
			auto &resCache = app.GetResourceCache();

			const Resource kResource(std::string(resource.begin(), resource.end()));
			auto pResourceHandle = resCache.GetHandle(kResource);
		});*/
	}

	void Deregister(void)
	{

	}
} // End namespace (BGE::ScriptExports)

namespace BGE
{
	void InternalScriptExports::ConsoleLog(sol::variadic_args args)
	{
		std::ostringstream oss;
		for (auto arg : args)
		{
			oss << arg.as<std::string>() << "\t";
		}
		// Log the collected output
		auto &app = GetEngineApp();
		auto &dbgConsole = app.GetDebugConsole();
		dbgConsole.AddToOutputLog(oss.str());
	}

	void InternalScriptExports::ConsoleOpen(void)
	{
		auto &app = GetEngineApp();
		auto &dbgConsole = app.GetDebugConsole();
		dbgConsole.SetEnabled(true);
	}

	void InternalScriptExports::ConsoleClose(void)
	{
		auto &app = GetEngineApp();
		auto &dbgConsole = app.GetDebugConsole();
		dbgConsole.SetEnabled(false);
	}

	void InternalScriptExports::LoggerFatal(sol::string_view fmt, sol::variadic_args args)
	{
		// Convert format string to C-string
		const char *pFormat = fmt.data();

		// Handle based on number of arguments
		switch (args.size())
		{
		case 0:
			BGE_FATAL(pFormat);
			break;
		case 1:
			BGE_FATAL(pFormat, args[0].as<std::string>().c_str());
			break;
		case 2:
			BGE_FATAL(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str());
			break;
		case 3:
			BGE_FATAL(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str());
			break;
		case 4:
			BGE_FATAL(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str(),
				args[3].as<std::string>().c_str());
			break;
		default:
			// If more arguments are provided than we handle, still log but with truncated args
			BGE_LOG("Lua", "Too many arguments provided to Fatal logger (max 4): %s", pFormat);
			break;
		}
	}

	void InternalScriptExports::LoggerFatalIf(bool bCondition, sol::string_view fmt, sol::variadic_args args)
	{
		if (bCondition)
		{
			LoggerFatal(fmt, args);
		}
	}

	void InternalScriptExports::LoggerError(sol::string_view fmt, sol::variadic_args args)
	{
		// Convert format string to C-string
		const char *pFormat = fmt.data();

		// Handle based on number of arguments
		switch (args.size())
		{
		case 0:
			BGE_ERROR(pFormat);
			break;
		case 1:
			BGE_ERROR(pFormat, args[0].as<std::string>().c_str());
			break;
		case 2:
			BGE_ERROR(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str());
			break;
		case 3:
			BGE_ERROR(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str());
			break;
		case 4:
			BGE_ERROR(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str(),
				args[3].as<std::string>().c_str());
			break;
		default:
			// If more arguments are provided than we handle, still log but with truncated args
			BGE_LOG("Lua", "Too many arguments provided to Error logger (max 4): %s", pFormat);
			break;
		}
	}

	void InternalScriptExports::LoggerErrorIf(bool bCondition, sol::string_view fmt, sol::variadic_args args)
	{
		if (bCondition)
		{
			LoggerError(fmt, args);
		}
	}

	void InternalScriptExports::LoggerWarning(sol::string_view fmt, sol::variadic_args args)
	{
// Convert format string to C-string
		const char *pFormat = fmt.data();

		// Handle based on number of arguments
		switch (args.size())
		{
		case 0:
			BGE_WARNING(pFormat);
			break;
		case 1:
			BGE_WARNING(pFormat, args[0].as<std::string>().c_str());
			break;
		case 2:
			BGE_WARNING(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str());
			break;
		case 3:
			BGE_WARNING(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str());
			break;
		case 4:
			BGE_WARNING(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str(),
				args[3].as<std::string>().c_str());
			break;
		default:
			// If more arguments are provided than we handle, still log but with truncated args
			BGE_LOG("Lua", "Too many arguments provided to Error logger (max 4): %s", pFormat);
			break;
		}
	}

	void InternalScriptExports::LoggerInfo(sol::string_view fmt, sol::variadic_args args)
	{
// Convert format string to C-string
		const char *pFormat = fmt.data();

		// Handle based on number of arguments
		switch (args.size())
		{
		case 0:
			BGE_INFO(pFormat);
			break;
		case 1:
			BGE_INFO(pFormat, args[0].as<std::string>().c_str());
			break;
		case 2:
			BGE_INFO(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str());
			break;
		case 3:
			BGE_INFO(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str());
			break;
		case 4:
			BGE_INFO(pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str(),
				args[3].as<std::string>().c_str());
			break;
		default:
			// If more arguments are provided than we handle, still log but with truncated args
			BGE_LOG("Lua", "Too many arguments provided to Error logger (max 4): %s", pFormat);
			break;
		}
	}

	void InternalScriptExports::LoggerLog(sol::string_view tag, sol::string_view fmt, sol::variadic_args args)
	{
		// Convert tag & format string to C-string
		const char *pTag = tag.data();
		const char *pFormat = fmt.data();

		// Handle based on number of arguments
		switch (args.size())
		{
		case 0:
			BGE_LOG(pTag, pFormat);
			break;
		case 1:
			BGE_LOG(pTag, pFormat, args[0].as<std::string>().c_str());
			break;
		case 2:
			BGE_LOG(pTag, pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str());
			break;
		case 3:
			BGE_LOG(pTag, pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str());
			break;
		case 4:
			BGE_LOG(pTag, pFormat,
				args[0].as<std::string>().c_str(),
				args[1].as<std::string>().c_str(),
				args[2].as<std::string>().c_str(),
				args[3].as<std::string>().c_str());
			break;
		default:
			// If more arguments are provided than we handle, still log but with truncated args
			BGE_LOG("Lua", "Too many arguments provided to Error logger (max 4): %s", pFormat);
			break;
		}
	}

	sol::string_view InternalScriptExports::LoggerLevelToString(int level)
	{
		const auto kLevelEnum = static_cast<Logger::Level>(level);
		return Logger::LevelToString(kLevelEnum);
	}
} // End namespace (BGE)
