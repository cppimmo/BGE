#ifndef _BGE_HUMANVIEW_HPP_
#define _BGE_HUMANVIEW_HPP_

#include "Events/EventData.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "UI/GameView.hpp"
#include "MainLoop/ProcessManager.hpp"

namespace BGE
{
	class IGameView; // Forward declare
	class HumanView; // Forward declare
	BGE_DECLARE_PTR(HumanView);

	/**
	 * @brief .
	 */
	class HumanView : public IGameView, public INonCopyable, public INonMovable
	{
		friend class EngineApp;

		/**
		 * @brief Handle keyboard inputs that should be processed for all human views.
		 */
		class DefaultInputHandler final : public IGamepadHandler, public IKeyboardHandler, public IMouseHandler
		{
			HumanView &m_humanView;
		public:
			DefaultInputHandler(HumanView &humanView);
			// IGamepadHandler's interface:
			virtual bool VOnAxis(JoystickID ID, GamepadAxis axis, std::int16_t value) override;
			virtual bool VOnButtonDown(JoystickID ID, GamepadButton button) override;
			virtual bool VOnButtonUp(JoystickID ID, GamepadButton button) override;
			virtual bool VOnGamepadConnected(JoystickID ID) override;
			virtual bool VOnGamepadDisconnected(JoystickID ID) override;
			// IKeyboardHandler's interface:
			virtual bool VOnKeyDown(SDL_Keycode key, bool bRepeat) override;
			virtual bool VOnKeyUp(SDL_Keycode key, bool bRepeat) override;
			// IMouseHandler's interface:
			virtual bool VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos) override;
			virtual bool VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll, const glm::fvec2 &kPreciseScroll) override;
			virtual bool VOnMouseButtonDown(const glm::ivec2 &kPos, MouseButton button, std::uint8_t clicks) override;
			virtual bool VOnMouseButtonUp(const glm::ivec2 &kPos, MouseButton button, std::uint8_t clicks) override;
			virtual int VGetPointerRadius(void) override;
		};
	protected:
		GameViewID m_viewID;
		ActorID m_actorID;
		BaseGameState m_state; //!< Current game state
		UniqueProcessManagerPtr m_pProcessManager;
		// TODO: Put audio system instance here.
		// TODO: Put renderer instance here.

		// Input handlers:
		GamepadHandlerList  m_gamepadHandlers;
		KeyboardHandlerList m_keyboardHandlers;
		MouseHandlerList    m_mouseHandlers;

		bool m_bInitialized = false; //!< Has the view been initialized?
	public:
		HumanView(void);
		virtual ~HumanView(void);
		//! IGameView's interface:
		virtual bool VInit(void) override;
		virtual void VOnRestore(void) override;
		virtual void VOnRender(float deltaTime, float elapsedTime) override;
		virtual void VOnLostDevice(void) override;
		virtual GameViewType VGetType(void) const override;
		virtual GameViewID VGetID(void) const override;
		virtual void VOnAttach(GameViewID viewID, ActorID actorID) override;
		virtual bool VOnHandleEvent(const SDL_Event &event) override;
		virtual void VOnUpdate(float deltaTime) override;
		virtual bool VIsInitialized(void) const override;

		void AddGamepadHandler(StrongIGamepadHandlerPtr pGamepadHandler);
		void AddKeyboardHandler(StrongIKeyboardHandlerPtr pKeyboardHandler);
		void AddMouseHandler(StrongIMouseHandlerPtr pMouseHandler);
		// Accessors:
		ProcessManager &GetProcessManager(void);
		// Event delegates:
		void PlaySoundDelegate(StrongIEventDataPtr pEventData);
		void GameStateDelegate(StrongIEventDataPtr pEventData);
	protected:
		virtual void VRegisterDelegates(void);
		virtual void VDeregisterDelegates(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_HUMANVIEW_HPP_ */
