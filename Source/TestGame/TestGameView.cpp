#include "TestGameStd.hpp"
#include "TestGameView.hpp"

namespace TestGame
{
	bool TestGameView::VInit(void)
	{
		m_pShaderProgram = std::make_unique<BGE::ShaderProgram>();

		m_pShaderProgram->VCreate();

		BGE::StrongIShaderPtr pVertexShader = std::make_shared<BGE::VertexShader>();
		pVertexShader->VCreate();
		pVertexShader->VCompile(R"vs(
#version 420 compatibility

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 g_vertexColor;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0);
	g_vertexColor = inColor;
}
)vs");

		BGE::StrongIShaderPtr pFragmentShader = std::make_shared<BGE::FragmentShader>();
		pFragmentShader->VCreate();
		pFragmentShader->VCompile(R"fs(
#version 420 compatibility

layout (location = 0) out vec4 outColor;

in vec3 g_vertexColor;

void main(void)
{
	outColor = vec4(g_vertexColor, 1.0);
}
)fs");

		m_pShaderProgram->VAttachShader(pVertexShader);
		m_pShaderProgram->VAttachShader(pFragmentShader);
		m_pShaderProgram->VLink();
		// Individual shaders can be destroyed now
		pVertexShader->VDestroy();
		pFragmentShader->VDestroy();

		//glm::fvec3 vertex(0.0f, 0.0f, 0.0f);
		static constexpr GLfloat vertices[3][3 + 3] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
			{  0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
			{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f }
		};

		glCreateBuffers(1, &m_vbo);
		glNamedBufferStorage(m_vbo, sizeof(vertices), vertices, 0);

		glCreateVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		return true;
	}

	void TestGameView::VOnRender(float deltaTime, float elapsedTime)
	{
		HumanView::VOnRender(deltaTime, elapsedTime);

		m_pShaderProgram->VBind();

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestGameView::VRegisterDelegates(void)
	{
	}

	void TestGameView::VDeregisterDelegates(void)
	{
	}
} // End namespace (TestGame)
