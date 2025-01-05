#ifndef _TG_TESTCONTROLLER_HPP_
#define _TG_TESTCONTROLLER_HPP_

#include <Input/Joystick.hpp>
#include <Input/Keyboard.hpp>
#include <Input/Mouse.hpp>
#include <Graphics/Camera.hpp>

namespace TestGame
{
	class TestController; // Forward declare
	BGE_DECLARE_PTR(TestController);

	class TestController : public BGE::IGamepadHandler, public BGE::IKeyboardHandler, public BGE::IMouseHandler
	{
	private:
		enum TestKey
		{
			kTEST_KEY_W,
			kTEST_KEY_A,
			kTEST_KEY_S,
			kTEST_KEY_D,
			kTEST_KEY_Q,
			kTEST_KEY_E,
			kTEST_KEY_COUNT
		};

		enum TestButtom
		{
			kTEST_BUTTON_A,
			kTEST_BUTTON_B,
			kTEST_BUTTON_COUNT
		};

		BGE::StrongFirstPersonCameraPtr m_pFPSCamera;
		float m_sensitivity;
		float m_gamepadSensitivity;
		float m_speed;
		float m_runSpeed;
		float m_yaw;
		float m_pitch;
		glm::fvec2 m_leftAxis;
		glm::fvec2 m_rightAxis;
		glm::ivec2 m_mouseMotion;
		std::array<bool, kTEST_KEY_COUNT> m_keys;
		std::array<bool, kTEST_BUTTON_COUNT> m_buttons;
		bool m_bRunning = false;
	public:
		TestController(BGE::StrongFirstPersonCameraPtr pFPSCamera);
		// IGamepadHandler's interface
		virtual bool VOnAxis(BGE::JoystickID ID, BGE::GamepadAxis axis, std::int16_t value) override;
		virtual bool VOnButtonDown(BGE::JoystickID ID, BGE::GamepadButton button) override;
		virtual bool VOnButtonUp(BGE::JoystickID ID, BGE::GamepadButton button) override;
		virtual bool VOnGamepadConnected(BGE::JoystickID ID) override;
		virtual bool VOnGamepadDisconnected(BGE::JoystickID ID) override;
		// IKeyboardHandler's interface
		virtual bool VOnKeyDown(SDL_Keysym key, bool bRepeat) override;
		virtual bool VOnKeyUp(SDL_Keysym key, bool bRepeat) override;
		// IMouseHandler's interface:
		virtual bool VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos) override;
		virtual bool VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll, const glm::fvec2 &kPreciseScroll) override;
		virtual bool VOnMouseButtonDown(const glm::ivec2 &kPos, BGE::MouseButton button, std::uint8_t clicks) override;
		virtual bool VOnMouseButtonUp(const glm::ivec2 &kPos, BGE::MouseButton button, std::uint8_t clicks) override;
		virtual int VGetPointerRadius(void) override;

		void OnUpdate(float deltaTime);
		const glm::fvec2 &GetRightAxis(void) const { return m_rightAxis; }
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTCONTROLLER_HPP_ */
