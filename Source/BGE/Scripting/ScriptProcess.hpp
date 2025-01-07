#ifndef _BGE_SCRIPTPROCESS_HPP_
#define _BGE_SCRIPTPROCESS_HPP_

#include "MainLoop/Process.hpp"

#include <sol/sol.hpp>

namespace BGE
{
	class ScriptProcess; // Forward declare
	BGE_DECLARE_PTR(ScriptProcess);

	class ScriptProcess : public Process
	{
		float m_frequency;
		float m_time;
		sol::function m_initFunc, m_updateFunc;
		sol::function m_successFunc, m_failFunc, m_abortFunc;
		sol::object m_self;
	public:
		ScriptProcess(void);
		virtual ~ScriptProcess(void) = default;

		static void RegisterScriptClass(sol::state &state);
	protected:
		// ScriptProcess's interface:
        virtual bool VOnInit(void) override;
        virtual void VOnUpdate(float deltaTime) override;
        virtual void VOnSuccess(void) override;
        virtual void VOnFail(void) override;
        virtual void VOnAbort(void) override;

		virtual bool BuildCppDataFromScript(sol::table scriptClass, sol::table constructionData) { return true; }
	private:
		static void RegisterScriptClassFunctions(sol::table &metatable);
		static sol::object CreateFromScript(sol::state &state, sol::table self, sol::table constructionData, sol::table originalSubClass);

		//! Wrapper member function for attaching child processes.
		void ScriptAttachChild(sol::object child);
	};
} // End namespace (BGE)

#endif /* !_BGE_SCRIPTPROCESS_HPP_ */
