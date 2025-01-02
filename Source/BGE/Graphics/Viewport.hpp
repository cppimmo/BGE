#ifndef _BGE_VIEWPORT_HPP_
#define _BGE_VIEWPORT_HPP_

namespace BGE
{
	class IViewport; // Forward declare
	BGE_DECLARE_PTR(IViewport);

	/**
	 * @brief .
	 */
	class IViewport
	{
	public:
		virtual ~IViewport(void) = default;
		// Interface:
		virtual glm::ivec2 VGetSize(void) const = 0;
		virtual glm::ivec2 VGetOffset(void) const = 0;
	};
} // End namespace

#endif /* !_BGE_VIEWPORT_HPP_ */
