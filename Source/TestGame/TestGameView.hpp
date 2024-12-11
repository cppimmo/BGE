#ifndef _TG_TESTGAMEVIEW_HPP_
#define _TG_TESTGAMEVIEW_HPP_

#include <UI/HumanView.hpp>

#include "TestController.hpp"

namespace TestGame
{
	class TestGameView : public BGE::HumanView
	{
	public:
		TestGameView(void)
		{
			auto pController = std::make_shared<TestController>();
			AddGamepadHandler(pController);
			AddKeyboardHandler(pController);
		}
		//! IGameView's interface:
	protected:
		virtual void VRegisterDelegates(void) override;
		virtual void VDeregisterDelegates(void) override;
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTGAMEVIEW_HPP_ */
