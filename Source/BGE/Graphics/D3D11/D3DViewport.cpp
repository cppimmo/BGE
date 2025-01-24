#include "Engine/EngineStd.hpp"
#include "Graphics/D3D11/D3DViewport.hpp"

namespace BGE
{
	D3DViewport::D3DViewport(const glm::ivec2 &kSize, const glm::ivec2 &kOffset)
		: m_size(kSize), m_offset(kOffset)
	{
		D3D11_VIEWPORT viewport = { };
		viewport.TopLeftX = static_cast<decltype(D3D11_VIEWPORT::TopLeftX)>(kOffset.x);
		viewport.TopLeftY = static_cast<decltype(D3D11_VIEWPORT::TopLeftX)>(kOffset.y);
		viewport.Width = static_cast<decltype(D3D11_VIEWPORT::Width)>(kSize.x);
		viewport.Height = static_cast<decltype(D3D11_VIEWPORT::Height)>(kSize.y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		m_viewport = viewport;
	}
	
	glm::ivec2 D3DViewport::VGetSize(void) const
	{
		return m_size;
	}
	
	glm::ivec2 D3DViewport::VGetOffset(void) const
	{
		return m_offset;
	}

	D3D11_VIEWPORT D3DViewport::GetD3D11Viewport(const D3DViewport &kViewport)
	{
		return kViewport.m_viewport;
	}
} // End namespace (BGE)
