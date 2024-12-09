#ifndef _BGE_KEYBOARD_HPP_
#define _BGE_KEYBOARD_HPP_

namespace BGE
{
	class IKeyboardHandler; // Forware declare
	BGE_DECLARE_PTR(IKeyboardHandler);

	using KeyboardHandlerList = std::list<StrongIKeyboardHandlerPtr>;

	class IKeyboardHandler
	{
	public:
		virtual ~IKeyboardHandler(void) = default;

		virtual bool VOnKeyDown(SDL_Keysym key, bool bRepeat) = 0;
		virtual bool VOnKeyUp(SDL_Keysym key, bool bRepeat) = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_KEYBOARD_HPP_ */
