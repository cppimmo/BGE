#ifndef _BGE_HUMANVIEW_HPP_
#define _BGE_HUMANVIEW_HPP_

#include "Events/EventData.hpp"
#include "Input/Joystick.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "UI/GameView.hpp"
#include "MainLoop/ProcessManager.hpp"

namespace BGE
{
	class IGameView; // Forward declare
	class HumanView; // Forward declare
	BGE_DECLARE_PTR(HumanView);

	class HumanView : public IGameView
	{
		friend class EngineApp;
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
	public:
		HumanView(void);
		virtual ~HumanView(void);
		//! IGameView's interface:
		virtual void VOnRestore(void) override;
		virtual void VOnRender(float deltaTime, float elapsedTime) override;
		virtual void VOnLostDevice(void) override;
		virtual GameViewType VGetType(void) override;
		virtual GameViewID VGetID(void) override;
		virtual void VOnAttach(GameViewID vID, ActorID aID) override;
		virtual bool VOnHandleEvent(const SDL_Event &kEvent) override;
		virtual void VOnUpdate(float deltaTime) override;
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
