#ifndef _BGE_GL_VIEWPORT_HPP_
#define _BGE_GL_VIEWPORT_HPP_

#include "Graphics/Viewport.hpp"

namespace BGE
{
	class GLViewport; // Forward declare
	BGE_DECLARE_PTR(GLViewport);

	/**
	 * @brief .
	 */
	class GLViewport final : public IViewport
	{
	private:
		glm::ivec2 m_size; //!< .
		glm::ivec2 m_offset; //!< .
	public:
		explicit GLViewport(const glm::ivec2 &kSize, const glm::ivec2 &kOffset = glm::ivec2(0));
		virtual ~GLViewport(void) override = default;
		// IViewport's interface:
		virtual glm::ivec2 VGetSize(void) const override;
		virtual glm::ivec2 VGetOffset(void) const override;
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_VIEWPORT_HPP_ */
