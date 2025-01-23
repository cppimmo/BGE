#ifndef _BGE_D3D11_D3D11RENDERER_HPP_
#define _BGE_D3D11_D3D11RENDERER_HPP_

#include <d3d11.h>
#include <dxgi1_3.h>

#include "Graphics/Renderer.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/ShaderBuffer.hpp"
//#include "Graphics/D3D11/ShaderBuffer.hpp"
//#include "Graphics/GL/Viewport.hpp"

namespace BGE
{
	class D3D11Renderer; // Forward declare
	BGE_DECLARE_PTR(D3D11Renderer);

	struct VertexPositionColor
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	template <ShaderBufferDataType Type>
	class D3DShaderBuffer; // Forward declare

	/**
	 * @brief .
	 */
	class D3D11Renderer : public IRenderer
	{
	private:
		//! Template alias for Microsoft's COM pointer.
		template <typename Type>
		using ComPtr = Microsoft::WRL::ComPtr<Type>;
		
		struct AdapterData
		{
			ComPtr<IDXGIAdapter1> pAdapter;
			DXGI_ADAPTER_DESC1 desc;
		};

		EngineOptions m_options; //!< .
		UniqueIViewportPtr m_pViewport; //!< .
		glm::vec4 m_bgColor; //! . // TODO: What should the default clear color be?
		ComPtr<ID3D11Device> m_pDevice = nullptr; //!< .
		ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr; //!< .
		ComPtr<IDXGIFactory2> m_pDXGIFactory = nullptr; //!< .
		ComPtr<IDXGISwapChain1> m_pSwapChain = nullptr; //!< .
		ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr; //!< .
		ComPtr<ID3D11DepthStencilView> m_pDepthTarget = nullptr; //!< .
		ComPtr<ID3D11DepthStencilState> m_pDepthState = nullptr; //!< .
		ComPtr<ID3D11RasterizerState> m_pSolidRasterState = nullptr; //!< .
		ComPtr<ID3D11RasterizerState> m_pWireframeRasterState = nullptr; //!< .
		ComPtr<ID3D11Debug> m_pDebug = nullptr; //!< .

		ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
		ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
		ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
		ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
		std::unique_ptr<D3DShaderBuffer<ShaderBufferData_WorldViewProjection>> m_pShaderBuffer = nullptr;

		D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_HARDWARE; //!< D3D driver type.
		ImGuiContext *m_pImGuiContext = nullptr; //< Pointer to ImGui context.
		ImPlotContext *m_pImPlotContext = nullptr; //< Pointer to ImPlot context.
		bool m_bInitialized = false; //!< Was the renderer intialized?
	public:
		D3D11Renderer(void);
		virtual ~D3D11Renderer(void) override;

		virtual bool VInit(const EngineOptions &kOptions) override;
		virtual void VShutdown(void) override;
		virtual RendererImpl VGetImpl(void) const override;

		virtual void VBeginFrame(void) override;
		virtual void VEndFrame(void) override;

		virtual void VOnResize(std::int32_t width, std::int32_t height) override;
		virtual void VSetViewport(const IViewport &kViewport) override;
		virtual const IViewport &VGetViewport(void) const override;
		virtual void VSetBackgroundColor(const glm::vec4 &kColor) override;
		virtual glm::vec4 VGetBackgroundColor(void) override;

		virtual void VEnableDepthTest(bool bEnable) override;
		virtual void VEnableBlending(bool bEnable) override;
		virtual void VSetBlendMode(BlendMode mode) override;
		virtual BlendMode VGetBlendMode(void) const override;

		virtual StrongIShaderFactoryPtr VCreateShaderFactory(void) override;
		virtual StrongIShaderProgramPtr VCreateShaderProgram(std::string_view name) override;
		virtual StrongIShaderProgramPtr VGetShaderProgram(std::string_view name) override;

		virtual bool VTakeScreenshot(const std::filesystem::path &kSaveGameDir) override;

		virtual void VEnableDebugOutput(bool bEnable) override;
		virtual std::string VGetRendererInfo(void) const override;

		static ID3D11Device *GetDevice(void) noexcept;
		static ID3D11DeviceContext *GetDeviceContext(void) noexcept;
	private:
		std::vector<AdapterData> EnumerateAdapters(void);
		bool CreateSwapchainResources(void);
		void DestroySwapchainResources(void);
		bool CompileShader(StrongResourceHandlePtr pResourceHandle, std::string_view entryPoint, std::string_view profile, ComPtr<ID3DBlob> &pShaderBlob);
		ComPtr<ID3D11VertexShader> CreateVertexShader(StrongResourceHandlePtr pResourceHandle, ComPtr<ID3DBlob> &pShaderBlob);
		ComPtr<ID3D11PixelShader> CreatePixelShader(StrongResourceHandlePtr pResourceHandle);
	};
} // End namespace (BGE)

#endif /* !_BGE_D3D11_D3D11RENDERER_HPP_ */
