/*=============================================================================*
 * Gamepad.cpp - Input from joysticks/gamepads.
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
#include "Engine/EngineStd.hpp"
#include "Input/Gamepad.hpp"

namespace BGE
{
	bool IGamepadHandler::StartRumble(JoystickID ID, RumbleSpeed sharedSpeed, RumbleDuration durationMS)
	{
		SDL_Gamepad *pGamepad = SDL_OpenGamepad(ID);
		if (!pGamepad) // && !SDL_GamepadHasRumble(pGamepad)
		{
			return false;
		}

		// Assume left motor is twice as powerful
		const auto kLeftSpeed = sharedSpeed;
		const auto kRightSpeed = sharedSpeed / 2;
		const int kResult = SDL_RumbleGamepad(pGamepad, kLeftSpeed, kRightSpeed, durationMS);
		return kResult == 0;
	}

	bool IGamepadHandler::StartRumble(JoystickID ID, RumbleSpeed leftSpeed, RumbleSpeed rightSpeed, RumbleDuration durationMS)
	{
		SDL_Gamepad *pGamepad = SDL_OpenGamepad(ID);
		if (!pGamepad) // && !SDL_GameControllerHasRumble(pGameController)
		{
			return false;
		}

		const int kResult = SDL_RumbleGamepad(pGamepad, leftSpeed, rightSpeed, durationMS);
		return kResult == 0;
	}

	bool IGamepadHandler::StopRumble(JoystickID ID)
	{
		SDL_Gamepad *pGameController = SDL_OpenGamepad(ID);
		if (!pGameController) // && !SDL_GameControllerHasRumble(pGameController)
		{
			return false;
		}

		const int kResult = SDL_RumbleGamepad(pGameController, 0u, 0u, 0u);
		return kResult == 0;
	}
} // End namespace (BGE)
