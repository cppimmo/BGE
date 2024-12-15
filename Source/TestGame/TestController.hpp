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
			using ShortLimits = std::numeric_limits<decltype(value)>;
			constexpr auto kMIN_VALUE = ShortLimits::min(), kMAX_VALUE = ShortLimits::max();

			const auto kNorm = BGE::Math::Normalize<float>(value, kMIN_VALUE, kMAX_VALUE);
			constexpr float kMAX_AXIS_VALUE = 1.0f;
			const auto kDeadzoned = BGE::Math::ApplyDeadzone<float>(kNorm, kMAX_AXIS_VALUE, 0.05f);

			switch (axis)
			{
			case BGE::kGAMEPAD_AXIS_LEFT_X:
				BGE_LOG("Input", "Axis left (x): %.2f", kNorm);
				return true;
				break;
			case BGE::kGAMEPAD_AXIS_LEFT_Y:
				BGE_LOG("Input", "Axis left (y): %.2f", kNorm);
				return true;
				break;
			case BGE::kGAMEPAD_AXIS_RIGHT_X:
				BGE_LOG("Input", "Axis right (x): %.2f", kNorm);
				return true;
				break;
			case BGE::kGAMEPAD_AXIS_RIGHT_Y:
				BGE_LOG("Input", "Axis right (y): %.2f", kNorm);
				return true;
				break;
			case BGE::kGAMEPAD_AXIS_TRIGGER_LEFT:
				BGE_LOG("Input", "Trigger left: %.2f", kNorm);
				return true;
				break;
			case BGE::kGAMEPAD_AXIS_TRIGGER_RIGHT:
				BGE_LOG("Input", "Trigger right: %.2f", kNorm);
				return true;
				break;
			default:
				break;
			}
			return false;
		}
		virtual bool VOnButtonDown(BGE::JoystickID ID, BGE::GamepadButton button) override
		{
			BGE_LOG("Input", "(%d) Gamepad button down: %d", ID, button);
			if (button == BGE::kGAMEPAD_BUTTON_X)
			{
				BGE_LOG("Input", "Gamepad X button down");
			}
			return false;
		}
		virtual bool VOnButtonUp(BGE::JoystickID ID, BGE::GamepadButton button) override
		{
			BGE_LOG("Input", "(%d) Gamepad button up: %d", ID, button);
			return false;
		}
		// IKeyboardHandler's interface
		virtual bool VOnKeyDown(SDL_Keysym key, bool bRepeat) override
		{
			BGE_LOG("Input", "Key down (%d), repeat %d", key, bRepeat);
			return false;
		}
		virtual bool VOnKeyUp(SDL_Keysym key, bool bRepeat) override
		{
			BGE_LOG("Input", "Key up (%d), repeat %d", key, bRepeat);
			return false;
		}
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTCONTROLLER_HPP_ */
