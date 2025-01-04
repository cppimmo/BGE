#ifndef _TG_TESTGAMEVIEW_HPP_
#define _TG_TESTGAMEVIEW_HPP_

#include <UI/HumanView.hpp>
#include <Graphics/GL/Shaders.hpp>
#include <Graphics/GL/ShaderProgram.hpp>
#include <Graphics/Transform.hpp>
#include <Graphics/Camera.hpp>

#include "TestController.hpp"
#include <glm/gtc/quaternion.hpp>

namespace TestGame
{
	class TestGameView; // Forward declare
	BGE_DECLARE_PTR(TestGameView);

	class TestGameView : public BGE::HumanView
	{
		struct Vertex
		{
			glm::vec3 position = glm::vec3(0);
			glm::vec3 normal = glm::vec3(0);
			glm::vec2 uv = glm::vec2(0);
		};

		BGE::UniqueIShaderProgramPtr m_pShaderProgram;
		GLuint m_vao, m_vbo;
		BGE::StrongIAudioSourcePtr m_pSource;
		BGE::UniqueCameraPtr m_pCamera;
		BGE::Transform m_triTransform;
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
