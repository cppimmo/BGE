#include "TestGameStd.hpp"
#include "TestController.hpp"

namespace TestGame
{
	TestController::TestController(BGE::StrongFirstPersonCameraPtr pFPSCamera)
		: m_pFPSCamera(pFPSCamera), m_sensitivity(0.75f), m_gamepadSensitivity(50.0f), m_speed(1.4f), m_runSpeed(5.0f), m_yaw(0.0f), m_pitch(0.0f)
	{
		m_leftAxis = glm::fvec2(0.0f);
		m_rightAxis = glm::fvec2(0.0f);
		m_mouseMotion = glm::ivec2(0);
		m_keys.fill(false); // Set the initial key states to off
		m_buttons.fill(false); // Set the initial button states to off
	}

	bool TestController::VOnAxis(BGE::JoystickID ID, BGE::GamepadAxis axis, std::int16_t value)
	{
		using ShortLimits = std::numeric_limits<decltype(value)>;
		constexpr auto kMIN_VALUE = ShortLimits::min(), kMAX_VALUE = ShortLimits::max();

		const auto kNorm = BGE::Math::Normalize<float>(value, kMIN_VALUE, kMAX_VALUE);
		constexpr float kMAX_AXIS_VALUE = 1.0f;
		const auto kDeadzoned = BGE::Math::ApplyDeadzone<float>(kNorm, kMAX_AXIS_VALUE, 0.25f);

		switch (axis)
		{
		case BGE::kGAMEPAD_AXIS_LEFT_X:
			BGE_LOG("Input", "Axis left (x): %.4f", kNorm);

			m_leftAxis.x = kDeadzoned;
			return true;
			break;
		case BGE::kGAMEPAD_AXIS_LEFT_Y:
			BGE_LOG("Input", "Axis left (y): %.4f", kNorm);

			m_leftAxis.y = kDeadzoned;
			return true;
			break;
		case BGE::kGAMEPAD_AXIS_RIGHT_X:
			BGE_LOG("Input", "Axis right (x): %.4f", kNorm);

			m_rightAxis.x = kDeadzoned;
			return true;
			break;
		case BGE::kGAMEPAD_AXIS_RIGHT_Y:
			BGE_LOG("Input", "Axis right (y): %.4f", kNorm);

			m_rightAxis.y = kDeadzoned;
			return true;
			break;
		case BGE::kGAMEPAD_AXIS_TRIGGER_LEFT:
			BGE_LOG("Input", "Trigger left: %.4f", kNorm);
			return true;
			break;
		case BGE::kGAMEPAD_AXIS_TRIGGER_RIGHT:
			BGE_LOG("Input", "Trigger right: %.4f", kNorm);
			return true;
			break;
		default:
			break;
		}
		return false;
	}

	bool TestController::VOnButtonDown(BGE::JoystickID ID, BGE::GamepadButton button)
	{
		using enum BGE::GamepadButton;
		BGE_LOG("Input", "(%d) Gamepad button down: %d", ID, button);

		if (button == kGAMEPAD_BUTTON_LEFT_STICK)
		{
			m_bRunning = true;
			return true;
		}
		else if (button == kGAMEPAD_BUTTON_A)
		{
			m_buttons[kTEST_BUTTON_A] = true;
			return true;
		}
		else if (button == kGAMEPAD_BUTTON_B)
		{
			m_buttons[kTEST_BUTTON_B] = true;
			return true;
		}

		switch (button)
		{
		case kGAMEPAD_BUTTON_X:
			BGE_LOG("Input", "Gamepad X button down");
			break;
		case kGAMEPAD_BUTTON_Y:
			BGE_LOG("Input", "Gamepad Y button down");
			break;
		case kGAMEPAD_BUTTON_A:
			BGE_LOG("Input", "Gamepad A button down");
			break;
		case kGAMEPAD_BUTTON_B:
			BGE_LOG("Input", "Gamepad B button down");
			break;
		case kGAMEPAD_BUTTON_BACK:
			BGE_LOG("Input", "Gamepad BACK button down");
			break;
		case kGAMEPAD_BUTTON_GUIDE:
			BGE_LOG("Input", "Gamepad GUIDE button down");
			break;
		case kGAMEPAD_BUTTON_START:
			BGE_LOG("Input", "Gamepad START button down");
			break;
		case kGAMEPAD_BUTTON_LEFT_STICK:
			BGE_LOG("Input", "Gamepad LEFT STICK button down");
			break;
		case kGAMEPAD_BUTTON_RIGHT_STICK:
			BGE_LOG("Input", "Gamepad RIGHT STICK button down");
		case kGAMEPAD_BUTTON_LEFT_SHOULDER:
			BGE_LOG("Input", "Gamepad LEFT SHOULDER button down");
			break;
		case kGAMEPAD_BUTTON_RIGHT_SHOULDER:
			BGE_LOG("Input", "Gamepad RIGHT_SHOULDER button down");
			break;
		case kGAMEPAD_BUTTON_DPAD_UP:
			BGE_LOG("Input", "Gamepad DPAD UP button down");
			break;
		case kGAMEPAD_BUTTON_DPAD_DOWN:
			BGE_LOG("Input", "Gamepad DPAD DOWN button down");
			break;
		case kGAMEPAD_BUTTON_DPAD_LEFT:
			BGE_LOG("Input", "Gamepad DPAD LEFT button down");
			break;
		case kGAMEPAD_BUTTON_DPAD_RIGHT:
			BGE_LOG("Input", "Gamepad DPAD RIGHT button down");
		default:
			break;
		}
		return false;
	}

	bool TestController::VOnButtonUp(BGE::JoystickID ID, BGE::GamepadButton button)
	{
		using enum BGE::GamepadButton;
		BGE_LOG("Input", "(%d) Gamepad button up: %d", ID, button);

		if (button == kGAMEPAD_BUTTON_LEFT_STICK)
		{
			m_bRunning = false;
			return true;
		}
		else if (button == kGAMEPAD_BUTTON_A)
		{
			m_buttons[kTEST_BUTTON_A] = false;
			return true;
		}
		else if (button == kGAMEPAD_BUTTON_B)
		{
			m_buttons[kTEST_BUTTON_B] = false;
			return true;
		}
		return false;
	}

	bool TestController::VOnGamepadConnected(BGE::JoystickID ID)
	{
		return false;
	}

	bool TestController::VOnGamepadDisconnected(BGE::JoystickID ID)
	{
		// Reset the left & right axis values
		m_leftAxis = glm::fvec2(0.0f);
		m_rightAxis = glm::fvec2(0.0f);
		return true;
	}

	bool TestController::VOnKeyDown(SDL_Keysym key, bool bRepeat)
	{
		BGE_LOG("Input", "Key down (%d), repeat: %s", key.sym, BGE::BoolToString(bRepeat).data());

		switch (key.sym)
		{
		case SDLK_w:
			m_keys[kTEST_KEY_W] = true;
			return true;
		case SDLK_a:
			m_keys[kTEST_KEY_A] = true;
			return true;
		case SDLK_s:
			m_keys[kTEST_KEY_S] = true;
			return true;
		case SDLK_d:
			m_keys[kTEST_KEY_D] = true;
			return true;
		case SDLK_q:
			m_keys[kTEST_KEY_Q] = true;
			return true;
		case SDLK_e:
			m_keys[kTEST_KEY_E] = true;
			return true;
		default:
			break;
		}

		if (key.sym == SDLK_LSHIFT)
		{
			m_bRunning = true;
			return true;
		}

		return false;
	}

	bool TestController::VOnKeyUp(SDL_Keysym key, bool bRepeat)
	{
		BGE_LOG("Input", "Key up (%d), repeat: %s", key.sym, BGE::BoolToString(bRepeat).data());

		switch (key.sym)
		{
		case SDLK_w:
			m_keys[kTEST_KEY_W] = false;
			return true;
		case SDLK_a:
			m_keys[kTEST_KEY_A] = false;
			return true;
		case SDLK_s:
			m_keys[kTEST_KEY_S] = false;
			return true;
		case SDLK_d:
			m_keys[kTEST_KEY_D] = false;
			return true;
		case SDLK_q:
			m_keys[kTEST_KEY_Q] = false;
			return true;
		case SDLK_e:
			m_keys[kTEST_KEY_E] = false;
			return true;
		default:
			break;
		}

		if (key.sym == SDLK_LSHIFT)
		{
			m_bRunning = false;
			return true;
		}

		return false;
	}

	bool TestController::VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos)
	{
		m_mouseMotion = kRelPos;
		return true;
	}

	bool TestController::VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll, const glm::fvec2 &kPreciseScroll)
	{
		return false;
	}

	bool TestController::VOnMouseButtonDown(const glm::ivec2 &kPos, BGE::MouseButton button, std::uint8_t clicks)
	{
		return false;
	}

	bool TestController::VOnMouseButtonUp(const glm::ivec2 &kPos, BGE::MouseButton button, std::uint8_t clicks)
	{
		return false;
	}

	int TestController::VGetPointerRadius(void)
	{
		return 0;
	}

	void TestController::OnUpdate(float deltaTime)
	{
		deltaTime /= 1000.0f; // Convert time delta to seconds
		//BGE_LOG("TestGame", "Speed: %.4f, dt: %.4f, speed * dt: %.4f", m_speed, deltaTime, m_speed * deltaTime);
		//BGE_LOG("TestGame", "Pos: (x: %07.4f, y: %07.4f, z: %07.4f)", m_pFPSCamera->GetPosition().x, m_pFPSCamera->GetPosition().y, m_pFPSCamera->GetPosition().z);
		//BGE_LOG("TestGame", "(%.8f, %.8f)", m_rightAxis.x, m_rightAxis.y);

		float speed = (m_bRunning ? m_runSpeed : m_speed);
		// Variable for movement deltas
		glm::vec3 movementDelta(0.0f);
		float yawDelta{}, pitchDelta{};

		// Gamepad input handling
		if (std::fabs(m_leftAxis.y) > 0.01f)
		{
			// Forward/backward movement (NOTE: The y axis needs to be inverted).
			if (m_leftAxis.y < 0.0f)
			{
				movementDelta += glm::vec3(0.0f, 0.0f, -m_leftAxis.y * speed * deltaTime);
			}
			else
			{
				movementDelta += glm::vec3(0.0f, 0.0f, -m_leftAxis.y * m_speed * deltaTime);
				// Can't run backwards
			}
		}
		if (std::fabs(m_leftAxis.x) > 0.01f)
		{
			movementDelta += glm::vec3(m_leftAxis.x * m_speed * deltaTime, 0.0f, 0.0f);
		}
		if (std::fabs(m_rightAxis.x) > 0.01f)
		{
			yawDelta += -m_rightAxis.x * m_gamepadSensitivity * deltaTime;
		}
		if (std::fabs(m_rightAxis.y) > 0.01f)
		{
			// Inverted Y-axis
			pitchDelta += m_rightAxis.y * m_gamepadSensitivity * deltaTime;
		}

		if (m_buttons[kTEST_BUTTON_A])
		{
			movementDelta -= glm::vec3(0.0f, m_speed * deltaTime, 0.0f);
		}
		if (m_buttons[kTEST_BUTTON_B])
		{
			movementDelta += glm::vec3(0.0f, m_speed * deltaTime, 0.0f);
		}

		// Left/right movement
		//m_pFPSCamera->MoveRight(m_leftAxis.x * m_speed * deltaTime);

		// Yaw rotation
		//BGE_LOG("TestGame", "Yaw calc: %08.7f", -m_rightAxis.x * m_sensitivity * deltaTime);
		//m_yaw += -m_rightAxis.x * m_sensitivity * deltaTime;
		//m_yaw = glm::mod(m_yaw, 360.0f); // Normalize yaw angle within [0, 360)
		//m_pFPSCamera->Rotate(m_yaw, m_pitch);

		// Pitch rotation
		//m_pitch -= -m_rightAxis.y * m_sensitivity * deltaTime; // Invert Y (the best)
		//m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
		//m_pFPSCamera->Rotate(m_yaw, m_pitch);

		// Keyboard input handling
		if (m_keys[kTEST_KEY_W])
		{
			movementDelta += glm::vec3(0.0f, 0.0f, speed * deltaTime); // Forward
		}
		if (m_keys[kTEST_KEY_A])
		{
			movementDelta -= glm::vec3(m_speed * deltaTime, 0.0f, 0.0f); // Left
		}
		if (m_keys[kTEST_KEY_S])
		{
			movementDelta -= glm::vec3(0.0f, 0.0f, m_speed * deltaTime); // Backward
		}
		if (m_keys[kTEST_KEY_D])
		{
			movementDelta += glm::vec3(m_speed * deltaTime, 0.0f, 0.0f); // Right
		}
		if (m_keys[kTEST_KEY_Q])
		{
			movementDelta -= glm::vec3(0.0f, m_speed * deltaTime, 0.0f);
		}
		if (m_keys[kTEST_KEY_E])
		{
			movementDelta += glm::vec3(0.0f, m_speed * deltaTime, 0.0f);
		}

		// Mouse input handling
		yawDelta -= static_cast<float>(m_mouseMotion.x) * m_sensitivity;
		pitchDelta -= static_cast<float>(m_mouseMotion.y) * m_sensitivity;
		m_mouseMotion = glm::ivec2(0);

		// Apply movement & rotation
		m_pFPSCamera->MoveForward(movementDelta.z);
		m_pFPSCamera->MoveUp(movementDelta.y);
		m_pFPSCamera->MoveRight(movementDelta.x);

		m_yaw = glm::mod(m_yaw + yawDelta, 360.0f); // Normalize yaw to [0, 360)
		m_pitch = glm::clamp(m_pitch + pitchDelta, -89.0f, 89.0f); // Clamp pitch
		m_pFPSCamera->Rotate(m_yaw, m_pitch);

		// Reset input axes for the next frame
		//m_leftAxis = glm::fvec2(0.0f);
		//m_rightAxis = glm::fvec2(0.0f);

		// Mouse:
		//m_yaw += m_mouseMotion.x * m_sensitivity * deltaTime;
		//m_pitch += m_mouseMotion.y * m_sensitivity * deltaTime;
		//m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
		//m_pFPSCamera->Rotate(m_yaw, m_pitch);
	}
} // End namespace (BGE)
