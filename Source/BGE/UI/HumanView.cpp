#include "Engine/EngineStd.hpp"
#include "HumanView.hpp"

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

bool BGE::HumanView::DefaultInputHandler::VOnKeyDown(SDL_Keycode key, bool bRepeat)
{
	if (key == SDLK_ESCAPE)
	{
		BGUTSendExitCode(kBGE_EXIT_SUCCESS);
		return true;
	}
	else if (key == SDLK_F5 && !bRepeat)
	{
		BGE_LOG("Input", "Taking screenshot...");
		auto &app = GetEngineApp();

		app.GetRenderer().VTakeScreenshot(app.VGetGameAppDirectory());
		return true;
	}
	else if (key == SDLK_GRAVE && !bRepeat)
	{
		auto &app = GetEngineApp();
		auto &dbgConsole = app.GetDebugConsole();
		// Toggle the debug console visibility
		dbgConsole.SetEnabled(!dbgConsole.IsEnabled());
		return true;
	}
	else if (key == SDLK_BACKSPACE && !bRepeat)
	{
		if (IMouseHandler::IsCursorLocked())
			IMouseHandler::LockMouseCursor();
		else
			IMouseHandler::ReleaseMouseCursor();
	}
	return false;
}

bool BGE::HumanView::DefaultInputHandler::VOnKeyUp(SDL_Keycode key, bool bRepeat)
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
	case SDL_EVENT_KEY_DOWN: // Keyboard events
		for (auto &pHandler : m_keyboardHandlers)
		{
			pHandler->VOnKeyDown(event.key.key, event.key.repeat);
		}
		break;
	case SDL_EVENT_KEY_UP:
		for (auto &pHandler : m_keyboardHandlers)
		{
			pHandler->VOnKeyUp(event.key.key, event.key.repeat);
		}
		break;
	case SDL_EVENT_TEXT_EDITING:
		break;
	case SDL_EVENT_TEXT_INPUT:
		break;
	case SDL_EVENT_KEYMAP_CHANGED:
		break;
	case SDL_EVENT_TEXT_EDITING_CANDIDATES:
		break;
	case SDL_EVENT_MOUSE_ADDED: // Mouse events
		break;
	case SDL_EVENT_MOUSE_REMOVED:
		break;
	case SDL_EVENT_MOUSE_MOTION: // Mouse events
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseMove(glm::ivec2(event.motion.x, event.motion.y),
								   glm::ivec2(event.motion.xrel, event.motion.yrel));
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseButtonDown(glm::ivec2(event.button.x, event.button.y),
										 static_cast<MouseButton>(event.button.button), event.button.clicks);
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseButtonUp(glm::ivec2(event.button.x, event.button.y),
									   static_cast<MouseButton>(event.button.button), event.button.clicks);
		}
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		for (auto &pHandler : m_mouseHandlers)
		{
			pHandler->VOnMouseWheel(glm::ivec2(event.wheel.mouse_x, event.wheel.mouse_y),
									glm::ivec2(event.wheel.x, event.wheel.y), glm::fvec2(event.wheel.x, event.wheel.y));
		}
		break;
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: // Game controller events
		for (auto &pHandler : m_gamepadHandlers)
		{
			// TODO: Handle return value.
			pHandler->VOnAxis(event.gaxis.which, static_cast<GamepadAxis>(event.gaxis.axis), event.gaxis.value);
		}
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnButtonDown(event.gbutton.which, static_cast<GamepadButton>(event.gbutton.button));
		}
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnButtonUp(event.gbutton.which, static_cast<GamepadButton>(event.gbutton.button));
		}
		break;
	case SDL_EVENT_GAMEPAD_ADDED:
		BGE_LOG("Input", "Gamepad device added (%d)", event.gdevice.which);

		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnGamepadConnected(event.gdevice.which);
		}
		break;
	case SDL_EVENT_GAMEPAD_REMOVED:
		BGE_LOG("Input", "Gamepad device removed (%d)", event.gdevice.which);

		for (auto &pHandler : m_gamepadHandlers)
		{
			pHandler->VOnGamepadDisconnected(event.gdevice.which);
		}
		break;
	case SDL_EVENT_GAMEPAD_REMAPPED:
		// What does this do?
		break;
	case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
		break;
	case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
		break;
	case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
		break;
	case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
		break;
	case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
		break;
	case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
		break;
	case SDL_EVENT_FINGER_DOWN: // Touch events
		break;
	case SDL_EVENT_FINGER_UP:
		break;
	case SDL_EVENT_FINGER_MOTION:
		break;
	case SDL_EVENT_CLIPBOARD_UPDATE: // Clipboard events
		break;
	case SDL_EVENT_DROP_FILE: // Drag and drop events
		break;
	case SDL_EVENT_DROP_TEXT:
		break;
	case SDL_EVENT_DROP_BEGIN:
		break;
	case SDL_EVENT_DROP_COMPLETE:
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
