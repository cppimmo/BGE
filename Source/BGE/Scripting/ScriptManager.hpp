#ifndef _BGE_SCRIPTMANAGER_HPP_
#define _BGE_SCRIPTMANAGER_HPP_

namespace BGE
{
	class IScriptManager; // Forward declare
	BGE_DECLARE_PTR(IScriptManager);

	class IScriptManager
	{
	public:
		virtual ~IScriptManager(void) = default;
		// Interface:
		virtual bool VInit(void) = 0;
		virtual void VExecuteString(std::string_view str) = 0;
		virtual void VExecuteFile(std::string_view filename) = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_SCRIPTMANAGER_HPP_ */
