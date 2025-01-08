#include "Engine/EngineStd.hpp"
#include "HumanView.hpp"

#include "Graphics/Screenshot.hpp"

BGE::HumanView::DefaultInputHandler::DefaultInputHandler(HumanView &humanView)
	: m_humanView(humanView)
{
}

bool BGE::HumanView::DefaultInputHandler::VOnAxis(JoystickID ID, GamepadAxis axis, std::int16_t value)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnButtonDown(JoystickID ID, GamepadButton button)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnButtonUp(JoystickID ID, GamepadButton button)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnGamepadConnected(JoystickID ID)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnGamepadDisconnected(JoystickID ID)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnKeyDown(SDL_Keysym key, bool bRepeat)
{
	if (key.sym == SDLK_ESCAPE)
	{
		BGUTSendExitCode(kBGE_EXIT_SUCCESS);
		return true;
	}
	else if (key.sym == SDLK_F5 && !bRepeat)
	{
		BGE_LOG("Input", "Taking screenshot...");
		auto &app = GetEngineApp();
		std::string saveGameDir = app.VGetGameAppDirectory();
		TakeScreenshot(saveGameDir);
		return true;
	}
	else if (key.sym == SDLK_BACKQUOTE && !bRepeat)
	{
		auto &app = GetEngineApp();
		auto &dbgConsole = app.GetDebugConsole();
		// Toggle the debug console visibility
		dbgConsole.SetEnabled(!dbgConsole.IsEnabled());
		return true;
	}
	else if (key.sym == SDLK_BACKSPACE && !bRepeat)
	{
		if (IMouseHandler::IsCursorLocked())
			IMouseHandler::LockMouseCursor();
		else
			IMouseHandler::ReleaseMouseCursor();
	}
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnKeyUp(SDL_Keysym key, bool bRepeat)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll,
														const glm::fvec2 &kPreciseScroll)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnMouseButtonDown(const glm::ivec2 &kPos, MouseButton button,
															 std::uint8_t clicks)
{
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnMouseButtonUp(const glm::ivec2 &kPos, MouseButton button,
														   std::uint8_t clicks)
{
	return false;
}

int BGE::HumanView::DefaultInputHandler::VGetPointerRadius(void)
{
	return 0;
}

BGE::HumanView::HumanView(void)
{
}

BGE::HumanView::~HumanView(void)
{
	m_bInitialized = false;
	// TODO: Add a dedicated shutdown member function to views
}

bool BGE::HumanView::VInit(void)
{
	// Add default input handlers
	auto pDefaultInputHandler = std::make_shared<DefaultInputHandler>(*this);
	AddGamepadHandler(pDefaultInputHandler);
	AddKeyboardHandler(pDefaultInputHandler);
	AddMouseHandler(pDefaultInputHandler);

	m_bInitialized = true;
	return true;
}

void BGE::HumanView::VOnRestore(void)
{
}

void BGE::HumanView::VOnRender(float deltaTime, float elapsedTime)
{
}

void BGE::HumanView::VOnLostDevice(void)
{
}

BGE::GameViewType BGE::HumanView::VGetType(void) const
{
	return GameViewType::Human;
}

BGE::GameViewID BGE::HumanView::VGetID(void) const
{
	return 0;
}

void BGE::HumanView::VOnAttach(GameViewID viewID, ActorID actorID)
{
}

bool BGE::HumanView::VOnHandleEvent(const SDL_Event &event)
{
	bool bResult = false;
	// TODO: Handle necessary SDL events.
	switch (event.type)
	{
	// Remaining cases forwarded to GameLogic/GameViews:
	case SDL_KEYDOWN: // Keyboard events
		for (auto &pHandler : m_keyboardHandlers)
		{
			pHandler->VOnKeyDown(event.key.keysym, event.key.repeat);
		}
		break;
	case SDL_KEYUP:
		for (auto &pHandler : m_keyboardHandlers)
		{
			pHandler->VOnKeyUp(event.key.keysym, event.key.repeat);
		}
		break;
	case SDL_TEXTEDITING:
		break;
	case SDL_TEXTINPUT:
		break;
	case SDL_KEYMAPCHANGED:
		break;
	case SDL_TEXTEDITING_EXT:
		break;
	case SDL_MOUSEMOTION: // Mouse events
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseMove(glm::ivec2(event.motion.x, event.motion.y),
								   glm::ivec2(event.motion.xrel, event.motion.yrel));
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseButtonDown(glm::ivec2(event.button.x, event.button.y),
										 static_cast<MouseButton>(event.button.button), event.button.clicks);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseButtonUp(glm::ivec2(event.button.x, event.button.y),
									   static_cast<MouseButton>(event.button.button), event.button.clicks);
		}
		break;
	case SDL_MOUSEWHEEL:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseWheel(glm::ivec2(event.wheel.mouseX, event.wheel.mouseY),
									glm::ivec2(event.wheel.x, event.wheel.y), glm::fvec2(event.wheel.preciseX, event.wheel.preciseY));
		}
		break;
	case SDL_JOYAXISMOTION: // Joystick events
		break;
	case SDL_JOYBALLMOTION:
		break;
	case SDL_JOYHATMOTION:
		break;
	case SDL_JOYBUTTONDOWN:
		break;
	case SDL_JOYBUTTONUP:
		break;
	case SDL_JOYDEVICEADDED:
		break;
	case SDL_JOYDEVICEREMOVED:
		break;
	case SDL_JOYBATTERYUPDATED:
		break;
	case SDL_CONTROLLERAXISMOTION: // Game controller events
		for (auto &pHandler : m_gamepadHandlers)
		{
			// TODO: Handle return value.
			pHandler->VOnAxis(event.caxis.which, static_cast<GamepadAxis>(event.caxis.axis), event.caxis.value);
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnButtonDown(event.cbutton.which, static_cast<GamepadButton>(event.cbutton.button));
		}
		break;
	case SDL_CONTROLLERBUTTONUP:
		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnButtonUp(event.cbutton.which, static_cast<GamepadButton>(event.cbutton.button));
		}
		break;
	case SDL_CONTROLLERDEVICEADDED:
		BGE_LOG("Input", "Gamepad device added (%d)", event.cdevice.which);

		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnGamepadConnected(event.cdevice.which);
		}
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		BGE_LOG("Input", "Gamepad device removed (%d)", event.cdevice.which);

		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnGamepadDisconnected(event.cdevice.which);
		}
		break;
	case SDL_CONTROLLERDEVICEREMAPPED:
		// What does this do?
		break;
	case SDL_CONTROLLERTOUCHPADDOWN:
		break;
	case SDL_CONTROLLERTOUCHPADMOTION:
		break;
	case SDL_CONTROLLERTOUCHPADUP:
		break;
	case SDL_CONTROLLERSENSORUPDATE:
		break;
	case SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3:
		break;
	case SDL_CONTROLLERSTEAMHANDLEUPDATED:
		break;
	case SDL_FINGERDOWN: // Touch events
		break;
	case SDL_FINGERUP:
		break;
	case SDL_FINGERMOTION:
		break;
	case SDL_DOLLARGESTURE: // Gesture events
		break;
	case SDL_DOLLARRECORD:
		break;
	case SDL_MULTIGESTURE:
		break;
	case SDL_CLIPBOARDUPDATE: // Clipboard events
		break;
	case SDL_DROPFILE: // Drag and drop events
		break;
	case SDL_DROPTEXT:
		break;
	case SDL_DROPBEGIN:
		break;
	case SDL_DROPCOMPLETE:
		break;
	default:
		break;
	}
	return bResult;
}

void BGE::HumanView::VOnUpdate(float deltaTime)
{
}

bool BGE::HumanView::VIsInitialized(void) const
{
	return m_bInitialized;
}

void BGE::HumanView::AddGamepadHandler(StrongIGamepadHandlerPtr pGamepadHandler)
{
	BGE_ASSERT(pGamepadHandler);
	m_gamepadHandlers.push_back(pGamepadHandler);
}

void BGE::HumanView::AddKeyboardHandler(StrongIKeyboardHandlerPtr pKeyboardHandler)
{
	BGE_ASSERT(pKeyboardHandler);
	m_keyboardHandlers.push_back(pKeyboardHandler);
}

void BGE::HumanView::AddMouseHandler(StrongIMouseHandlerPtr pMouseHandler)
{
	BGE_ASSERT(pMouseHandler);
	m_mouseHandlers.push_back(pMouseHandler);
}

BGE::ProcessManager &BGE::HumanView::GetProcessManager(void)
{
	BGE_ASSERT(m_pProcessManager);
	return *m_pProcessManager.get();
}

void BGE::HumanView::PlaySoundDelegate(StrongIEventDataPtr pEventData)
{
}

void BGE::HumanView::GameStateDelegate(StrongIEventDataPtr pEventData)
{
}

void BGE::HumanView::VRegisterDelegates(void)
{
}

void BGE::HumanView::VDeregisterDelegates(void)
{
}
