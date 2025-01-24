/*=============================================================================*
 * Gamepad.hpp - Input from joysticks/gamepads.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *============================================================================*/
#ifndef _BGE_INPUT_GAMEPAD_HPP_
#define _BGE_INPUT_GAMEPAD_HPP_

namespace BGE
{
	//! .
	using JoystickID = SDL_JoystickID;
	
	//! .
	enum GamepadAxis : int
	{
		kGAMEPAD_AXIS_LEFT_X = SDL_GAMEPAD_AXIS_LEFTX,
		kGAMEPAD_AXIS_LEFT_Y = SDL_GAMEPAD_AXIS_LEFTY,
		kGAMEPAD_AXIS_RIGHT_X = SDL_GAMEPAD_AXIS_RIGHTX,
		kGAMEPAD_AXIS_RIGHT_Y = SDL_GAMEPAD_AXIS_RIGHTY,
		kGAMEPAD_AXIS_TRIGGER_LEFT = SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
		kGAMEPAD_AXIS_TRIGGER_RIGHT = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
		kGAMEPAD_AXIS_COUNT = SDL_GAMEPAD_AXIS_COUNT
	};
	constexpr int kGAMEPAD_AXIS_INVALID = SDL_GAMEPAD_AXIS_INVALID;

	inline constexpr std::string_view GamepadAxisToString(GamepadAxis axis)
	{
		switch (axis)
		{
		case kGAMEPAD_AXIS_LEFT_X:
			return "GAMEPAD_AXIS_LEFT_X";
		case kGAMEPAD_AXIS_LEFT_Y:
			return "GAMEPAD_AXIS_LEFT_Y";
		case kGAMEPAD_AXIS_RIGHT_X:
			return "GAMEPAD_AXIS_RIGHT_X";
		case kGAMEPAD_AXIS_RIGHT_Y:
			return "GAMEPAD_AXIS_RIGHT_Y";
		case kGAMEPAD_AXIS_TRIGGER_LEFT:
			return "GAMEPAD_AXIS_TRIGGER_LEFT";
		case kGAMEPAD_AXIS_TRIGGER_RIGHT:
			return "GAMEPAD_AXIS_TRIGGER_RIGHT";
		default:
			return "Unknown";
		}
	}
	
	//! .
	enum GamepadButton : int
	{
		kGAMEPAD_BUTTON_A = SDL_GAMEPAD_BUTTON_SOUTH,
		kGAMEPAD_BUTTON_B = SDL_GAMEPAD_BUTTON_EAST,
		kGAMEPAD_BUTTON_X = SDL_GAMEPAD_BUTTON_WEST,
		kGAMEPAD_BUTTON_Y = SDL_GAMEPAD_BUTTON_NORTH,
		kGAMEPAD_BUTTON_BACK = SDL_GAMEPAD_BUTTON_BACK,
		kGAMEPAD_BUTTON_GUIDE = SDL_GAMEPAD_BUTTON_GUIDE,
		kGAMEPAD_BUTTON_START = SDL_GAMEPAD_BUTTON_START,
		kGAMEPAD_BUTTON_LEFT_STICK = SDL_GAMEPAD_BUTTON_LEFT_STICK,
		kGAMEPAD_BUTTON_RIGHT_STICK = SDL_GAMEPAD_BUTTON_RIGHT_STICK,
		kGAMEPAD_BUTTON_LEFT_SHOULDER = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
		kGAMEPAD_BUTTON_RIGHT_SHOULDER = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
		kGAMEPAD_BUTTON_DPAD_UP = SDL_GAMEPAD_BUTTON_DPAD_UP,
		kGAMEPAD_BUTTON_DPAD_DOWN = SDL_GAMEPAD_BUTTON_DPAD_DOWN,
		kGAMEPAD_BUTTON_DPAD_LEFT = SDL_GAMEPAD_BUTTON_DPAD_LEFT,
		kGAMEPAD_BUTTON_DPAD_RIGHT = SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
		kGAMEPAD_BUTTON_MISC_1 = SDL_GAMEPAD_BUTTON_MISC1,
		kGAMEPAD_BUTTON_TOUCHPAD = SDL_GAMEPAD_BUTTON_TOUCHPAD,
		kGAMEPAD_BUTTON_COUNT = SDL_GAMEPAD_BUTTON_COUNT
	};
	constexpr int kGAMEPAD_BUTTON_INVALID = SDL_GAMEPAD_BUTTON_INVALID;

	class IGamepadHandler; // Forward declare
	BGE_DECLARE_PTR(IGamepadHandler);

	//! .
	using GamepadHandlerList = std::list<StrongIGamepadHandlerPtr>;

	//! Rumble speed.
	using RumbleSpeed = std::uint16_t;
	inline constexpr RumbleSpeed kMIN_RUMBLE_SPEED = std::numeric_limits<RumbleSpeed>::min();
	inline constexpr RumbleSpeed kMAX_RUMBLE_SPEED = std::numeric_limits<RumbleSpeed>::max();
	//! Rumble duration is milliseconds.
	using RumbleDuration = std::uint32_t;
	inline constexpr RumbleDuration kDEFAULT_RUMBLE_DURATION = 10;

	/**
	 * @brief .
	 */
	class IGamepadHandler
	{
	public:
		virtual ~IGamepadHandler(void) = default;

		virtual bool VOnAxis(JoystickID ID, GamepadAxis axis, std::int16_t value) = 0;
		virtual bool VOnButtonDown(JoystickID ID, GamepadButton button) = 0;
		virtual bool VOnButtonUp(JoystickID ID, GamepadButton button) = 0;
		virtual bool VOnGamepadConnected(JoystickID ID) = 0;
		virtual bool VOnGamepadDisconnected(JoystickID ID) = 0;
		static bool StartRumble(JoystickID ID, RumbleSpeed sharedSpeed, RumbleDuration durationMS);
		static bool StartRumble(JoystickID ID, RumbleSpeed leftSpeed, RumbleSpeed rightSpeed, RumbleDuration durationMS);
		static bool StopRumble(JoystickID ID);
	};
} // End namespace (BGE)

#endif /* !_BGE_INPUT_GAMEPAD_HPP_ */
