#include "TestGameStd.hpp"
#include "TestGameView.hpp"

using namespace TestGame;

bool TestGame::TestGameView::VInit(void)
{
	glm::fvec3 vertex(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::fvec3), &vertex, GL_STATIC_DRAW);
	return true;
}

void TestGame::TestGameView::VOnRender(float deltaTime, float elapsedTime)
{
	HumanView::VOnRender(deltaTime, elapsedTime);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(0));
	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);
}

void TestGame::TestGameView::VRegisterDelegates(void)
{
}

void TestGame::TestGameView::VDeregisterDelegates()
{
}

