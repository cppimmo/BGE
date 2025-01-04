#ifndef _BGE_SCRIPTEXPORTS_HPP_
#define _BGE_SCRIPTEXPORTS_HPP_

#include <sol/sol.hpp>

namespace BGE
{
	//! .
	namespace ScriptExports
	{
		void Register(void);
		void Deregister(void);
	}

	/**
	 * @brief .
	 */
	class InternalScriptExports : public INonCopyable, public INonMovable
	{
		// Register & Deregister of ScriptExports are friend functions
		friend void ScriptExports::Register(void);
		friend void ScriptExports::Deregister(void);
	private:
		// Console table/module:
		static void ConsoleLog(sol::variadic_args args);
		static void ConsoleOpen(void);
		static void ConsoleClose(void);
		static bool ConsoleIsEnabled(void);
		// Logger table/module:
		static void LoggerFatal(sol::string_view fmt, sol::variadic_args args);
		static void LoggerFatalIf(bool bCondition, sol::string_view fmt, sol::variadic_args args);
		static void LoggerError(sol::string_view fmt, sol::variadic_args args);
		static void LoggerErrorIf(bool bCondition, sol::string_view fmt, sol::variadic_args args);
		static void LoggerWarning(sol::string_view fmt, sol::variadic_args args);
		static void LoggerInfo(sol::string_view fmt, sol::variadic_args args);
		static void LoggerLog(sol::string_view tag, sol::string_view fmt, sol::variadic_args args);
		static sol::string_view LoggerLevelToString(int level);
	};
} // End namespace (BGE)

#endif /* !_BGE_SCRIPTEXPORTS_HPP_ */
