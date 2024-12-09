#ifndef _BGE_MOUSE_HPP_
#define _BGE_MOUSE_HPP_

namespace BGE
{
	class IMouseHandler; // Forward declare
	BGE_DECLARE_PTR(IMouseHandler);

	using MouseHandlerList = std::list<StrongIMouseHandlerPtr>;

	class IMouseHandler
	{
	public:
		virtual ~IMouseHandler(void) = default;

		//virtual bool VOnMouseMove(const glm::ivec2 &kPos, const glm::ivec2 &kRelPos) = 0;
		//virtual bool VOnMouseWheel(const glm::ivec2 &kPos, const glm::ivec2 &kScroll, const glm::ivec2 &kPreciseScroll) = 0;
		//virtual bool VOnMouseButtonDown(const glm::ivec &kPos, std::uint8_t button, std::uint8_t clicks);
		//virtual bool VOnMouseButtonUp(const glm::ivec &kPos, std::uint8_t button, std::uint8_t clicks);
		virtual int VGetPointerRadius(void) = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_MOUSE_HPP_ */
