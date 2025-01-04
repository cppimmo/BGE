#ifndef _BGE_UI_RMLUI_SDLSYSTEMINTERFACE_HPP_
#define _BGE_UI_RMLUI_SDLSYSTEMINTERFACE_HPP_

#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/Types.h>

namespace BGE
{
	class SDLSystemInterface; // Forward declare
	BGE_DECLARE_PTR(SDLSystemInterface);

	/**
	 * @brief .
	 */
	class SDLSystemInterface : public Rml::SystemInterface
	{
	private:
		SDL_Window *m_pWindow = nullptr;
		SDL_Cursor *m_pCursorDefault = nullptr;
		SDL_Cursor *m_pCursorMove = nullptr;
		SDL_Cursor *m_pCursorPointer = nullptr;
		SDL_Cursor *m_pCursorResize = nullptr;
		SDL_Cursor *m_pCursorCross = nullptr;
		SDL_Cursor *m_pCursorText = nullptr;
		SDL_Cursor *m_pCursorUnavailable = nullptr;
	public:
		SDLSystemInterface(void);
		~SDLSystemInterface(void) override;
		// Rml::SystemInterface's interace:
		virtual double GetElapsedTime(void) override;
		virtual void SetMouseCursor(const Rml::String &cursorName) override;
		virtual void SetClipboardText(const Rml::String &text) override;
		virtual void GetClipboardText(Rml::String &text) override;
		virtual void ActivateKeyboard(Rml::Vector2f caretPosition, float lineHeight) override;
		virtual void DeactivateKeyboard(void) override;
		// Optionally, provide or change the window to be used for setting the mouse cursors.
		void SetWindow(SDL_Window *pWindow);
	public:
		static bool InputEventHandler(Rml::Context *pContext, SDL_Window *pWindow, SDL_Event &event);
		static Rml::Input::KeyIdentifier ConvertKey(int sdlKey);
		static int ConvertMouseButton(int sdlMouseButton);
		static int GetKeyModifierState(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_UI_RMLUI_SDLSYSTEMINTERFACE_HPP_ */
