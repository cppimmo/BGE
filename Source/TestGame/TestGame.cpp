#include <Engine/EngineStd.hpp>
#include "TestGame.hpp"

using namespace BGE;

int main(int numArgs, char *pArgs[])
{
	// Create the application layer:
	if (!CreateEngineApp<TestGameApp>())
		return BGE_EXIT_FAILURE;

	return EngineMain(numArgs, pArgs);
}
// Tank Battles application layer implementation:
std::string TestGameApp::VGetGameTitle(void)
{
	return "Tank Battles";
}

std::string TestGameApp::VGetGameAppDirectory(void)
{
	auto saveGameDir = GetSaveGameDirectory("cppimmo", SnakeCaseString(VGetGameTitle()));
	if (!saveGameDir)
	{
		auto currDirectory = GetCurrentDirectory();
		BGE_ASSERT(currDirectory.has_value());
		return *currDirectory;
	}
	return *saveGameDir;
}

std::string TestGameApp::VGetIcon(void)
{
	return "TankBattles.ico";
}

void TestGameApp::VRegisterGameEvents(void)
{
}

void TestGameApp::VCreateNetworkEventForwarder(void)
{
}

void TestGameApp::VDestroyNetworkEventForwarder(void)
{
}

BaseGameLogic *TestGameApp::VCreateGameAndView(void)
{
	return nullptr;
}

int TestGameApp::VGetRendererImpl(void)
{
	return 0;
}
