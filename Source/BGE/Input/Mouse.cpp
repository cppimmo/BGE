#include "Engine/EngineStd.hpp"
#include "Mouse.hpp"

namespace BGE
{
	bool IMouseHandler::s_bCursorLocked = false;

	void IMouseHandler::LockMouseCursor(void)
	{
		s_bCursorLocked = true;
		SDL_SetWindowRelativeMouseMode(BGUTGetWindowPtr(), true);
	}

	void IMouseHandler::ReleaseMouseCursor(void)
	{
		s_bCursorLocked = false;
		SDL_SetWindowRelativeMouseMode(BGUTGetWindowPtr(), false);
	}

	bool IMouseHandler::IsCursorLocked(void)
	{
		return s_bCursorLocked;
	}
} // End namespace (BGE)
