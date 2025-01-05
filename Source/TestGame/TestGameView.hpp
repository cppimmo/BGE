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
		BGE::StrongFirstPersonCameraPtr m_pCamera;
		BGE::Transform m_triTransform;
		StrongTestControllerPtr m_pController;
	public:
		TestGameView(void);
		//! IGameView's interface:
		virtual bool VInit(void) override;
		virtual void VOnRender(float deltaTime, float elapsedTime) override;
		virtual void VOnUpdate(float deltaTime) override;
	protected:
		virtual void VRegisterDelegates(void) override;
		virtual void VDeregisterDelegates(void) override;
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTGAMEVIEW_HPP_ */
