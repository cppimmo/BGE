#ifndef _BGE_GRAPHICS_D3D11_D3DSHADERS_HPP_
#define _BGE_GRAPHICS_D3D11_D3DSHADERS_HPP_

#include "Graphics/Shaders.hpp"
#include "Graphics/D3D11/D3DRenderer.hpp"

namespace BGE
{
	class D3DShader; // Forward declare
	BGE_DECLARE_PTR(D3DShader);

	/**
	 * @brief . 
	 */
	class D3DShader final : public IShader
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_pShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pShaderBlob; //!< Holds compiled shader binary.
	public:
		D3DShader(void) = default;

		~D3DShader(void) override
		{
			VDestroy();
		}

		// IShader's interface:
		virtual bool VCreate(void) override
		{
			return true;
		}

		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) override
		{
			BGE_ASSERT(pResourceHandle->GetType() == ResourceType::kHLSL);

			HRESULT hr;

			UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if BGE_CONFIG_DEBUG
			compileFlags |= D3DCOMPILE_DEBUG;
#endif /* BGE_CONFIG_DEBUG */

			Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob = nullptr;

			auto pExtraData = std::static_pointer_cast<PlaintextResourceExtraData>(pResourceHandle->GetExtraData());
			std::string shaderSource = pExtraData->VGetExtraData();

			constexpr std::string_view kENTRY_POINT = "Main";
			std::string profile;
			switch (m_type)
			{
			case ShaderType::kVertex:
				profile = "vs_5_0";
				break;
			case ShaderType::kPixel:
				profile = "ps_5_0";
				break;
			default:
				BGE_ASSERT(false);
				break;
			}

			hr = D3DCompile(
				shaderSource.c_str(), shaderSource.size(),
				nullptr, nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				kENTRY_POINT.data(), profile.data(),
				compileFlags, 0u,
				&m_pShaderBlob, &pErrorBlob);
			if (FAILED(hr))
			{
				std::ostringstream oss;
				oss << "D3D11: Failed to compile shader from source";

				if (pErrorBlob)
				{
					oss << " with message: " << static_cast<const char *>(pErrorBlob->GetBufferPointer());
				}

				std::string message = oss.str();
				BGE_ERROR("%s", message.c_str());
				return false;
			}

			switch (m_type)
			{
			case ShaderType::kVertex:
				hr = D3DRenderer::GetDevice()->CreateVertexShader(
					m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(),
					nullptr, reinterpret_cast<ID3D11VertexShader **>(m_pShader.ReleaseAndGetAddressOf()));
				break;
			case ShaderType::kPixel:
				hr = D3DRenderer::GetDevice()->CreatePixelShader(
					m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(),
					nullptr, reinterpret_cast<ID3D11PixelShader **>(m_pShader.ReleaseAndGetAddressOf()));
				break;
			default:
				BGE_ASSERT(false);
				break;
			}
			

			return SUCCEEDED(hr);
		}

		virtual void *VGetBlob(void) const override
		{
			if (!m_pShaderBlob)
			{
				return nullptr;
			}

			return m_pShaderBlob.Get();
		}

		virtual void VDestroy(void) override
		{
			m_pShader.Reset();
			m_pShaderBlob.Reset();
		}

		virtual bool VIsValid(void) const override
		{
			return m_pShader != nullptr;
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_D3D11_D3DSHADERS_HPP_ */