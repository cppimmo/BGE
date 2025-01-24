#ifndef _TG_TESTGAMEVIEW_HPP_
#define _TG_TESTGAMEVIEW_HPP_

#include <BGE/UI/HumanView.hpp>
#include <BGE/Graphics/GL/Shaders.hpp>
#include <BGE/Graphics/GL/ShaderProgram.hpp>
#include <BGE/Graphics/Transform.hpp>
#include <BGE/Graphics/Camera.hpp>
#include <BGE/Graphics/Geometry.hpp>

#include "TestController.hpp"
#include <glm/gtc/quaternion.hpp>

namespace TestGame
{
	class TestGameView; // Forward declare
	BGE_DECLARE_PTR(TestGameView);

	class TestGameView : public BGE::HumanView
	{
	private:
#if 0
		BGE::UniqueIShaderProgramPtr m_pShaderProgram, m_pSkyboxProgram;
		GLuint m_vao{}, m_vbo{}, m_ebo{};
		GLuint m_skyboxVao{}, m_skyboxVbo{}, m_skyboxEbo{};
		BGE::StrongIAudioSourcePtr m_pSource;
		BGE::StrongFirstPersonCameraPtr m_pCamera;
		BGE::Transform m_triTransform;
		StrongTestControllerPtr m_pController;
		BGE::MeshData m_meshData, m_skyboxMeshData;
		GLuint m_textureID{};
#endif
		void TestCreate(GLuint &vao, GLuint &vbo, GLuint &ebo, BGE::MeshData &meshData);
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
