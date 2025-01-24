#ifndef _TG_TESTPROCESSES_HPP_
#define _TG_TESTPROCESSES_HPP_

#include <BGE/Events/EventData.hpp>

namespace TestGame
{
	class TestProcess : public BGE::Process
	{
		BGE::Timer m_timer;
	public:
		virtual bool VOnInit(void) override;
		virtual void VOnUpdate(float deltaTime) override;
		virtual void VOnSuccess(void) override;
		virtual void VOnAbort(void) override;
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTPROCESSES_HPP_ */
