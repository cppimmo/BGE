#ifndef _BGE_GRAPHICS_D3D11_D3DINDEXBUFFER_HPP_
#define _BGE_GRAPHICS_D3D11_D3DINDEXBUFFER_HPP_

#include "Graphics/IndexBuffer.hpp"
#include "Graphics/D3D11/D3DRenderer.hpp"

namespace BGE
{
	template <typename IndexType>
	requires std::integral<IndexType> // Ensure only integral types (e.g., std::uint16_t, std::uint32_t)
	class D3DIndexBuffer final : public IIndexBuffer<IndexType>
	{
	private:
		Michaelsoft::ComPtr<ID3D11Buffer> m_pBuffer;
		std::size_t m_indexCount = 0;
	public:
		D3DIndexBuffer(void) = default;

		~D3DIndexBuffer(void) override
		{
			VDestroy();
		}

		virtual bool VCreate(const std::vector<IndexType> &kIndices, bool bDynamic) override
		{
			HRESULT hr;

			D3D11_BUFFER_DESC bufferDesc = { };
			bufferDesc.Usage = bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = static_cast<UINT>(kIndices.size() * sizeof(IndexType));
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = bDynamic ? D3D11_CPU_ACCESS_WRITE : 0u;

			D3D11_SUBRESOURCE_DATA subData = { };
			subData.pSysMem = kIndices.data();

			hr = D3DRenderer::GetDevice()->CreateBuffer(&bufferDesc, &subData, &m_pBuffer);
			m_indexCount = kIndices.size();
			return SUCCEEDED(hr);
		}

		virtual void VBind(void) const override
		{
			UINT offset = 0;
			D3DRenderer::GetDeviceContext()->IASetIndexBuffer(
				m_pBuffer.Get(),
				((sizeof(IndexType) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), offset);
		}

		virtual void VUpdate(const std::vector<IndexType> &kIndices) override
		{
			if (!m_pBuffer)
			{
				return;
			}

			D3D11_MAPPED_SUBRESOURCE mappedResource = { };
			if (SUCCEEDED(D3DRenderer::GetDeviceContext()->Map(m_pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource)))
			{
				std::memcpy(mappedResource.pData, kIndices.data(), kIndices.size() * sizeof(IndexType));
				D3DRenderer::GetDeviceContext()->Unmap(m_pBuffer.Get(), 0u);
				m_indexCount = kIndices.size();
			}
		}
		
		virtual void VDestroy(void) override
		{
			m_pBuffer.Reset();
		}

		virtual std::size_t VGetIndexCount(void) const noexcept override
		{
			return m_indexCount;
		}

		virtual std::uintptr_t VGetNativeHandle(void) const override
		{
			return reinterpret_cast<std::uintptr_t>(m_pBuffer.Get());
		}
	};
}

#endif /* !_BGE_GRAPHICS_D3D11_D3DINDEXBUFFER_HPP_ */