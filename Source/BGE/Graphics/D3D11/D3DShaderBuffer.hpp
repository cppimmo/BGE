/*=============================================================================*
 * ShaderBuffer.hpp - .
 *
 * Copyright (c) 2025, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *============================================================================*/
#ifndef _BGE_GRAPHICS_D3D11_D3DSHADERBUFFER_HPP_
#define _BGE_GRAPHICS_D3D11_D3DSHADERBUFFER_HPP_

#include <d3d11.h>

#include "Graphics/ShaderBuffer.hpp"
#include "Graphics/D3D11/D3DRenderer.hpp"

namespace BGE
{
	template <ShaderBufferDataType Type>
	class D3DShaderBuffer; // Forward declare
	BGE_DECLARE_MULTITEMPLATED_PTR(D3DShaderBuffer);

	/**
	 * @brief Wrapper around a shader buffer object. Can be applied to DirectX 11's constant
	 * buffers or OpenGL's uniform buffers.
	 * @tparam Type
	 */
	template <ShaderBufferDataType Type>
	class D3DShaderBuffer final : public IShaderBuffer<Type>
	{
		using IShaderBuffer<Type>::m_shaderType;
		using IShaderBuffer<Type>::m_bufferData;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer = nullptr;
	public:
		explicit D3DShaderBuffer(ShaderType shaderType) : IShaderBuffer<Type>(shaderType) { }
		D3DShaderBuffer(ShaderType shaderType, const Type &kData) : IShaderBuffer<Type>(shaderType, kData) { }
		~D3DShaderBuffer(void) override { VDestroy(); }
		// IShaderBuffer's interface:
		virtual bool VCreate(void) override
		{
			D3D11_BUFFER_DESC bufferDesc = { };
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Type) + (16 - (sizeof(Type) % 16)));
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0u;
			bufferDesc.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA subData = { };
			subData.pSysMem = &m_bufferData;

			HRESULT hr = D3DRenderer::GetDevice()->CreateBuffer(&bufferDesc, &subData, &m_pBuffer);
			if (FAILED(hr))
			{
				BGE_ERROR("Failed to create shader buffer.");
				return false;
			}

			return true;
		}

		virtual void Update(const Type &kData) override
		{
			m_bufferData = kData;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			D3DRenderer::GetDeviceContext()->Map(m_pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource);
			std::memcpy(mappedResource.pData, reinterpret_cast<const void *>(&m_bufferData), sizeof(Type));
			D3DRenderer::GetDeviceContext()->Unmap(m_pBuffer.Get(), 0u);
		}

		virtual void VBind(std::uint32_t slot) const override
		{
			switch (m_shaderType)
			{
			case ShaderType::kVertex:
				D3DRenderer::GetDeviceContext()->VSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			case ShaderType::kHull:
				D3DRenderer::GetDeviceContext()->HSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			case ShaderType::kDomain:
				D3DRenderer::GetDeviceContext()->DSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			case ShaderType::kGeometry:
				D3DRenderer::GetDeviceContext()->GSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			case ShaderType::kPixel:
				D3DRenderer::GetDeviceContext()->PSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			case ShaderType::kCompute:
				D3DRenderer::GetDeviceContext()->CSSetConstantBuffers(slot, 1u, m_pBuffer.GetAddressOf());
				break;
			default:
				BGE_ASSERT(false && "Invalid shader type");
				break;
			}
		}

		virtual void VDestroy(void) override
		{
			m_pBuffer.Reset();
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_D3D11_D3DSHADERBUFFER_HPP_ */
