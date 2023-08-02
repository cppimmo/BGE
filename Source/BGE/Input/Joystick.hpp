/*=============================================================================*
 * Joystick.hpp - Input from joysticks/gamepads.
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
#ifndef _BGE_JOYSTICK_HPP_
#define _BGE_JOYSTICK_HPP_

namespace BGE
{

using JoystickId = int;
	
class IJoystick
{
public:
	virtual bool VIsConnected(void) = 0;
	virtual JoystickId VGetId(void) = 0;
	virtual std::string VGetName(void) = 0;
	virtual std::string VGetSerial(void) = 0;
protected:
	virtual float VApplyDeadzone(float value, float maxValue, float deadzone) = 0;
};

enum class GamepadAxis : int
{
	LeftX = SDL_CONTROLLER_AXIS_LEFTX,
	LeftY = SDL_CONTROLLER_AXIS_LEFTY,
	RightX = SDL_CONTROLLER_AXIS_RIGHTX,
	RightY = SDL_CONTROLLER_AXIS_RIGHTY,
	TriggerLeft = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
	TriggerRight = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
	Count = SDL_CONTROLLER_AXIS_MAX
};
constexpr int kGAMEPAD_AXIS_INVALID = SDL_CONTROLLER_AXIS_INVALID;
	
enum class GamepadButton : int
{
	A = SDL_CONTROLLER_BUTTON_A,
	B = SDL_CONTROLLER_BUTTON_B,
	X = SDL_CONTROLLER_BUTTON_X,
	Y = SDL_CONTROLLER_BUTTON_Y,
	Back = SDL_CONTROLLER_BUTTON_BACK,
	Guide = SDL_CONTROLLER_BUTTON_GUIDE,
	Start = SDL_CONTROLLER_BUTTON_START,
	LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
	RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	DPadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
	DPadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	DPadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	DPadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	Misc1 = SDL_CONTROLLER_BUTTON_MISC1,
	Paddle1 = SDL_CONTROLLER_BUTTON_PADDLE1,
	Paddle2 = SDL_CONTROLLER_BUTTON_PADDLE2,
	Paddle3 = SDL_CONTROLLER_BUTTON_PADDLE3,
	Paddle4 = SDL_CONTROLLER_BUTTON_PADDLE4,
	Touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	Count = SDL_CONTROLLER_BUTTON_MAX
};
constexpr int kGAMEPAD_BUTTON_INVALID = SDL_CONTROLLER_BUTTON_INVALID;

class Gamepad : public IJoystick, public ISDLEventHandler
{
public:
	virtual void VHandleEvent_SDL(const SDL_Event &event) {}
	virtual bool VIsConnected(void) { return false; }
	virtual JoystickId VGetId(void) { return 0; }
	virtual std::string VGetName(void) { return ""; }
	virtual std::string VGetSerial(void) { return ""; }
	virtual float VApplyDeadzone(float value, float maxValue, float deadzone) { return 0.0f; }
};

} // End namespace (BGE)

#endif /* !_BGE_JOYSTICK_HPP_ */