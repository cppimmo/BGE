#include "Engine/EngineStd.hpp"
#include "HumanView.hpp"

#include "Graphics/Screenshot.hpp"

BGE::HumanView::DefaultKeyboardHandler::DefaultKeyboardHandler(HumanView &humanView)
	: m_humanView(humanView)
{
}

bool BGE::HumanView::DefaultKeyboardHandler::VOnKeyDown(SDL_Keysym key, bool bRepeat)
{
	if (key.sym == SDLK_ESCAPE)
	{
		BGUTSendExitCode(BGE_EXIT_SUCCESS);
		return true;
	}
	else if (key.sym == SDLK_s && !bRepeat)
	{
		BGE_LOG("Input", "Taking screenshot...");
		static bool c_initialized = false;
		if (!c_initialized)
		{
			auto &app = GetEngineApp();
			std::string saveGameDir = app.VGetGameAppDirectory();
			TakeScreenshot(saveGameDir);
			BGE_INFO("Tried to take screenshot!");
			c_initialized = true;
		}
		return true;
	}
	return false;
}

bool BGE::HumanView::DefaultKeyboardHandler::VOnKeyUp(SDL_Keysym key, bool bRepeat)
{
	return false;
}

BGE::HumanView::HumanView(void)
{
	// Add default keyboard handler
	auto pDefaultKeyboardHandler = std::make_shared<DefaultKeyboardHandler>(*this);
	AddKeyboardHandler(pDefaultKeyboardHandler);
}

BGE::HumanView::~HumanView(void)
{
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

BGE::GameViewType BGE::HumanView::VGetType(void)
{
	return GameViewType::kHuman;
}

BGE::GameViewID BGE::HumanView::VGetID(void)
{
	return 0;
}

void BGE::HumanView::VOnAttach(GameViewID vID, ActorID aID)
{
}

bool BGE::HumanView::VOnHandleEvent(const SDL_Event &kEvent)
{
	bool bResult = false;
	// TODO: Handle necessary SDL events.
	switch (kEvent.type)
	{
	// Remaining cases forwarded to GameLogic/GameViews:
	case SDL_KEYDOWN: // Keyboard events
		for (auto &handler : m_keyboardHandlers)
		{
			handler->VOnKeyDown(kEvent.key.keysym, kEvent.key.repeat);
		}
		break;
	case SDL_KEYUP:
		for (auto &handler : m_keyboardHandlers)
		{
			handler->VOnKeyUp(kEvent.key.keysym, kEvent.key.repeat);
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
		for (auto &handler : m_mouseHandlers)
		{
			//handler->VOnMouseMoved();
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		for (auto &handler : m_mouseHandlers)
		{
			//handler->VOnMouseButtonDown();
		}
		break;
	case SDL_MOUSEBUTTONUP:
		for (auto &handler : m_mouseHandlers)
		{
			//handler->VOnMouseButtonUp();
		}
		break;
	case SDL_MOUSEWHEEL:
		for (auto &handler : m_mouseHandlers)
		{
			//handler->VOnMouseWheel();
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
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		for (auto &handler : m_gamepadHandlers)
		{
			handler->VOnButtonDown(kEvent.cbutton.which, static_cast<GamepadButton>(kEvent.cbutton.button));
		}
		break;
	case SDL_CONTROLLERBUTTONUP:
		for (auto &handler : m_gamepadHandlers)
		{
			handler->VOnButtonUp(kEvent.cbutton.which, static_cast<GamepadButton>(kEvent.cbutton.button));
		}
		break;
	case SDL_CONTROLLERDEVICEADDED:
		BGE_LOG("Input", "Gamepad device added");
		// TODO: Do something with this event.
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		BGE_LOG("Input", "Gamepad device removed");
		// TODO: Do something with this event.
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
