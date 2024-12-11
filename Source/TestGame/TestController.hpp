#ifndef _TG_TESTCONTROLLER_HPP_
#define _TG_TESTCONTROLLER_HPP_

#include <Input/Joystick.hpp>
#include <Input/Keyboard.hpp>
#include <Input/Mouse.hpp>

namespace TestGame
{
	class TestController : public BGE::IGamepadHandler, public BGE::IKeyboardHandler
	{
	public:
		// IGamepadHandler's interface
		virtual bool VOnAxis(BGE::JoystickID ID, BGE::GamepadAxis axis, std::int16_t value) override
		{
			return true;
		}
		virtual bool VOnButtonDown(BGE::JoystickID ID, BGE::GamepadButton button) override
		{
			BGE_LOG("Input", "(%d) Gamepad button down: %d", ID, button);
			if (button == BGE::kGAMEPAD_BUTTON_X)
			{
				BGE_LOG("Input", "Gamepad X button down");
			}
			return true;
		}
		virtual bool VOnButtonUp(BGE::JoystickID ID, BGE::GamepadButton button) override
		{
			BGE_LOG("Input", "(%d) Gamepad button up: %d", ID, button);
			return true;
		}
		// IKeyboardHandler's interface
		virtual bool VOnKeyDown(SDL_Keysym key, bool bRepeat) override
		{
			BGE_LOG("Input", "Key down (%d), repeat %b", key, bRepeat);
			return true;
		}
		virtual bool VOnKeyUp(SDL_Keysym key, bool bRepeat) override
		{
			BGE_LOG("Input", "Key up (%d), repeat %b", key, bRepeat);
			return true;
		}
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTCONTROLLER_HPP_ */
