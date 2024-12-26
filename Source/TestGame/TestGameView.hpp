#ifndef _TG_TESTGAMEVIEW_HPP_
#define _TG_TESTGAMEVIEW_HPP_

#include <UI/HumanView.hpp>
#include <Graphics/Shaders.hpp>
#include <Graphics/ShaderProgram.hpp>

#include "TestController.hpp"

namespace TestGame
{
	class TestGameView : public BGE::HumanView
	{
		BGE::UniqueShaderProgramPtr m_pShaderProgram;
		GLuint m_vao, m_vbo;
	public:
		TestGameView(void)
		{
			auto pController = std::make_shared<TestController>();
			AddGamepadHandler(pController);
			AddKeyboardHandler(pController);
			VInit();
		}
		//! IGameView's interface:
		virtual bool VInit(void) override;
		virtual void VOnRender(float deltaTime, float elapsedTime) override;
	protected:
		virtual void VRegisterDelegates(void) override;
		virtual void VDeregisterDelegates(void) override;
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTGAMEVIEW_HPP_ */
