#ifndef _TG_TESTGAMEVIEW_HPP_
#define _TG_TESTGAMEVIEW_HPP_

#include <UI/HumanView.hpp>
#include <Graphics/Shaders.hpp>
#include <Graphics/ShaderProgram.hpp>

#include "TestController.hpp"
#include <glm/gtc/quaternion.hpp>

namespace TestGame
{
	class TestGameView : public BGE::HumanView
	{
		struct Vertex
		{
			glm::vec3 position = glm::vec3(0);
			glm::vec3 normal = glm::vec3(0);
			glm::vec2 uv = glm::vec2(0);
		};

		struct Transform
		{
			glm::vec3 position = glm::vec3(0);
			glm::vec3 rotation = glm::vec3(0);
			glm::vec3 scale = glm::vec3(1);
			glm::mat4 ToMat4(void)
			{
				glm::mat4 m = glm::translate(glm::mat4(1), position);
				m *= glm::mat4_cast(glm::quat(rotation));
				m = glm::scale(m, scale);
				return m;
			}
		};

		BGE::UniqueShaderProgramPtr m_pShaderProgram;
		GLuint m_vao, m_vbo;
		BGE::StrongIAudioSourcePtr m_pSource;
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
