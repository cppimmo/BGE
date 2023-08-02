#include <Engine/EngineStd.hpp>
#include "TankBattles.hpp"

using namespace BGE;

int main(int argc, char *argv[])
{
	// Create the application layer:
	if (!CreateEngineApp<TankBattlesApp>())
		return BGE_EXIT_FAILURE;
	return EngineMain(argc, argv);
}
// Tank Battles application layer implementation:
std::string TankBattlesApp::VGetGameTitle(void)
{
	return "Tank Battles";
}

std::string TankBattlesApp::VGetGameAppDirectory(void)
{
	auto saveGameDir = GetSaveGameDirectory("cppimmo", "Tank Battles");
	if (!saveGameDir)
	{
		auto currDirectory = GetCurrentDirectory();
		BGE_ASSERT(currDirectory.has_value());
		return *currDirectory;
	}
	return *saveGameDir;
}

std::string TankBattlesApp::VGetIcon(void)
{
	return "TankBattles.ico";
}

void TankBattlesApp::VRegisterGameEvents(void)
{
}

void TankBattlesApp::VCreateNetworkEventForwarder(void)
{
}

void TankBattlesApp::VDestroyNetworkEventForwarder(void)
{
}

BaseGameLogic *TankBattlesApp::VCreateGameAndView(void)
{
	return nullptr;
}

int TankBattlesApp::VGetRendererImpl(void)
{
	return 0;
}