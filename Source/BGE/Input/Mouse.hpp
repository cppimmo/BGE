#ifndef _BGE_INPUT_MOUSE_HPP_
#define _BGE_INPUT_MOUSE_HPP_

namespace BGE
{
	class IMouseHandler; // Forward declare
	BGE_DECLARE_PTR(IMouseHandler);

	//! .
	using MouseHandlerList = std::list<StrongIMouseHandlerPtr>;

	//! .
	enum MouseButton : int
	{
		kMOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
		kMOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
		kMOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
		kMOUSE_BUTTON_X1 = SDL_BUTTON_X1,
		kMOUSE_BUTTON_X2 = SDL_BUTTON_X2,
		kMOUSE_BUTTON_INVALID = -1
	};

	/**
	 * @brief .
	 */
	class IMouseHandler
	{
	private:
		static bool s_bCursorLocked; //!< Is the mouse cursor locked to the window?
	public:
		virtual ~IMouseHandler(void) = default;
		// Interface:
		virtual bool VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos) = 0;
		virtual bool VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll, const glm::fvec2 &kPreciseScroll) = 0;
		virtual bool VOnMouseButtonDown(const glm::ivec2 &kPos, MouseButton button, std::uint8_t clicks) = 0;
		virtual bool VOnMouseButtonUp(const glm::ivec2 &kPos, MouseButton button, std::uint8_t clicks) = 0;
		virtual int VGetPointerRadius(void) = 0;
	public:
		static void LockMouseCursor(void);
		static void ReleaseMouseCursor(void);
		static bool IsCursorLocked(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_INPUT_MOUSE_HPP_ */
