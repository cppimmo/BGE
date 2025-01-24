#ifndef _BGE_GRAPHICS_D3D11_D3DVIEWPORT_HPP_
#define _BGE_GRAPHICS_D3D11_D3DVIEWPORT_HPP_

#include <d3d11.h>

#include "Graphics/Viewport.hpp"

namespace BGE
{
	class D3DViewport; // Forward declare
	BGE_DECLARE_PTR(D3DViewport);

	/**
	 * @brief .
	 */
	class D3DViewport final : public IViewport
	{
	private:
		D3D11_VIEWPORT m_viewport;
		glm::ivec2 m_size; //!< .
		glm::ivec2 m_offset; //!< .
	public:
		explicit D3DViewport(const glm::ivec2 &kSize, const glm::ivec2 &kOffset = glm::ivec2(0));
		virtual ~D3DViewport(void) override = default;
		// IViewport's interface:
		virtual glm::ivec2 VGetSize(void) const override;
		virtual glm::ivec2 VGetOffset(void) const override;

		static D3D11_VIEWPORT GetD3D11Viewport(const D3DViewport &kViewport);
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_D3D11_D3DVIEWPORT_HPP_ */
