#include "Engine/EngineStd.hpp"
#include "Graphics/D3D11/Renderer.hpp"

#include <SDL_syswm.h>
#include <d3dcompiler.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_dx11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

//#include "Graphics/GL/Shaders.hpp"
//#include "Graphics/GL/ShaderProgram.hpp"
#include "Graphics/GL/Viewport.hpp"

namespace BGE
{
	D3D11Renderer::D3D11Renderer(void)
		: m_options{},
		  m_pViewport(std::make_unique<GLViewport>(glm::ivec2(1280, 720))),
		  m_bgColor(0.1f, 0.1f, 0.1f, 1.0f)
	{
	}

	D3D11Renderer::~D3D11Renderer(void)
	{
		VShutdown();
	}

	bool D3D11Renderer::VInit(const EngineOptions &kOptions)
	{
		m_options = kOptions; // Set the options

		HRESULT hr = S_OK;

		// Create the DXGI factory
		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory))))
		{
			BGE_ERROR("DXGI: Unable to create DXGIFactory");
			return false;
		}

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if BGE_CONFIG_DEBUG
		// If the project is in a debug build & renderer debug is enabled, enable the debug layer.
		if (*m_options.bRendererDebug)
		{
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif // BGE_CONFIG_DEBUG

		// Create the device and device context
		constexpr std::array<D3D_FEATURE_LEVEL, 1> kDEVICE_FEATURE_LEVELS = 
		{
			D3D_FEATURE_LEVEL_11_0
		};
		
		if (FAILED(D3D11CreateDevice(
			nullptr, // Specify nullptr to use the default adapter
			m_driverType,
			nullptr, // Specify nullptr to indicate use of D3D_DRIVER_TYPE_HARDWARE 
			creationFlags,
			kDEVICE_FEATURE_LEVELS.data(),
			kDEVICE_FEATURE_LEVELS.size(),
			D3D11_SDK_VERSION, // Always set to D3D11_SDK_VERSION
			&m_pDevice,
			nullptr,
			&m_pDeviceContext)))
		{
			BGE_ERROR("D3D11: Failed to create device and device context");
			return false;
		}

#if BGE_CONFIG_DEBUG
		if (*m_options.bRendererDebug && FAILED(m_pDevice.As(&m_pDebug)))
		{
			BGE_ERROR("D3D11: Failed to get the debug layer from the device");
			return false;
		}
#endif // BGE_CONFIG_DEBUG

		int width{ }, height{ };
		BGUTGetWindowSize(BGUTGetWindowPtr(), width, height);

		// Configure multisampling
		DXGI_SAMPLE_DESC sampleDesc = { };
		sampleDesc.Count = 1u;
		sampleDesc.Quality = 0u;

		constexpr DXGI_FORMAT kSWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
		
		// Check if the user enabled multisampling 
		if (*m_options.MSAA > 0)
		{
			UINT qualityLevelCount = 0;
			if (FAILED(m_pDevice->CheckMultisampleQualityLevels(
				kSWAPCHAIN_FORMAT,
				*m_options.MSAA,
				&qualityLevelCount)))
			{
				BGE_LOG("D3D11", "Choosen multisampling quality level not supported, using defaults");
			}
			else
			{
				sampleDesc.Count = *m_options.MSAA;
				sampleDesc.Quality = qualityLevelCount - 1;
			}
		}

		// TODO: When initializing the depth buffer, use the same sample descriptor.

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { };
		swapChainDesc.Width = static_cast<decltype(DXGI_SWAP_CHAIN_DESC1::Width)>(width);
		swapChainDesc.Height = static_cast<decltype(DXGI_SWAP_CHAIN_DESC1::Height)>(height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // kSWAPCHAIN_FORMAT;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2u;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // DXGI_SWAP_EFFECT_FLIP_DISCARD
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Flags = { };

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = { };
		swapChainFullscreenDesc.Windowed = true;
		
		auto pWindow = BGUTGetWindowPtr();
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(pWindow, &wmInfo);
		HWND hWnd = reinterpret_cast<HWND>(wmInfo.info.win.window);

		if (FAILED(m_pDXGIFactory->CreateSwapChainForHwnd(
			m_pDevice.Get(),
			hWnd,
			&swapChainDesc,
			&swapChainFullscreenDesc,
			nullptr,
			&m_pSwapChain)))
		{
			BGE_ERROR("DXGI: Failed to create swapchain.");
			return false;
		}

		// Create the swap chain & its associated resources
		if (!CreateSwapchainResources())
		{
			return false;
		}

		if (*kOptions.bImGuiEnabled)
		{
			IMGUI_CHECKVERSION(); // What does this do?

			// Create ImGui context
			if (!(m_pImGuiContext = ImGui::CreateContext()))
			{
				BGE_ERROR("BGUTInitImGui Failure: Couldn't create ImGui context!");
				return false;
			}
			// Create ImPlot context
			if (!(m_pImPlotContext = ImPlot::CreateContext()))
			{
				BGE_ERROR("BGUTInitImGui Failure: Couldn't create ImPlot context!");
				return false;
			}

			BGUTSetImGuiContextPtrs(m_pImGuiContext, m_pImPlotContext);

			ImGuiIO &io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
			//io.IniFilename = nullptr; // TODO: Set ImGui config filename/location.
			
			// Set ImGui style colors
			ImGui::StyleColorsDark();
			
			// Setup platform/renderer backends
			if (!ImGui_ImplSDL2_InitForD3D(BGUTGetWindowPtr()))
			{
				BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize SDL2 implementation!");
				return false;
			}

			// TODO: Define the GLSL version string elsewhere.
			if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get()))
			{
				BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize OpenGL3 implementation!");
				return false;
			}
		}

		D3D11_RASTERIZER_DESC rasterizerStateDesc = { };
		rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
		rasterizerStateDesc.FrontCounterClockwise = false;
		rasterizerStateDesc.DepthClipEnable = true;

		hr = m_pDevice->CreateRasterizerState(&rasterizerStateDesc, &m_pSolidRasterState);
		if (FAILED(hr))
		{
			return false;
		}

		rasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
		rasterizerStateDesc.FrontCounterClockwise = false;
		rasterizerStateDesc.DepthClipEnable = true;

		hr = m_pDevice->CreateRasterizerState(&rasterizerStateDesc, &m_pWireframeRasterState);
		if (FAILED(hr))
		{
			return false;
		}

		auto &app = GetEngineApp();
		auto &resCache = app.GetResourceCache();
		ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
		m_pVertexShader = CreateVertexShader(resCache.GetHandle(Resource("Assets\\Shaders\\test_vert.hlsl")), pVertexShaderBlob);
		if (!m_pVertexShader)
		{
			return false;
		}

		m_pPixelShader = CreatePixelShader(resCache.GetHandle(Resource("Assets\\Shaders\\test_pixl.hlsl")));
		if (!m_pPixelShader)
		{
			return false;
		}

		constexpr D3D11_INPUT_ELEMENT_DESC kVERTEX_INPUT_LAYOUT_INFO[] =
		{
			{
				"POSITION",
				0u,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0u,
				offsetof(VertexPositionColor, position),
				D3D11_INPUT_PER_VERTEX_DATA,
				0u
			},
			{
				"COLOR",
				0u,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0u,
				offsetof(VertexPositionColor, color),
				D3D11_INPUT_PER_VERTEX_DATA,
				0u
			}
		};

		if (FAILED(m_pDevice->CreateInputLayout(
			kVERTEX_INPUT_LAYOUT_INFO,
			std::size(kVERTEX_INPUT_LAYOUT_INFO),
			pVertexShaderBlob->GetBufferPointer(),
			pVertexShaderBlob->GetBufferSize(),
			&m_pInputLayout)))
		{
			BGE_ERROR("D3D11: Failed to create default vertex input layout");
			return false;
		}

		constexpr VertexPositionColor kVERTICES[] =
		{
			{ glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
			{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
			{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC bufferDesc = { };
		bufferDesc.ByteWidth = sizeof(kVERTICES);
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.pSysMem = kVERTICES;

		if (FAILED(m_pDevice->CreateBuffer(&bufferDesc, &subresourceData, &m_pVertexBuffer)))
		{
			BGE_ERROR("D3D11: Failed to create triangle vertex buffer");
			return false;
		}

		m_bInitialized = true; // Set the initialization flag
		return true;
	}

	void D3D11Renderer::VShutdown(void)
	{
		// Check for proper initialization
		if (!m_bInitialized)
		{
			return;
		}

		if (*m_options.bImGuiEnabled)
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImPlot::DestroyContext(); // Destroy ImPlot context first
			ImGui::DestroyContext();
		}

		m_pDeviceContext->Flush();
		DestroySwapchainResources();
		m_pSwapChain.Reset();
		m_pDXGIFactory.Reset();
		m_pDeviceContext.Reset();
#if BGE_CONFIG_DEBUG
		if (*m_options.bRendererDebug)
		{
			m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			m_pDebug.Reset();
		}
#endif
		m_pDevice.Reset();
		m_bInitialized = false; // Set the initialization flag
	}

	RendererImpl D3D11Renderer::VGetImpl(void) const
	{
		return RendererImpl::kD3D11;
	}

	void D3D11Renderer::VBeginFrame(void)
	{
		BGE_ASSERT(m_pDeviceContext);
		BGE_ASSERT(m_pSwapChain);

		// When ImGui is enabled, prepare the new frame
		if (*m_options.bImGuiEnabled)
		{
			ImGui::SetCurrentContext(m_pImGuiContext);
			ImPlot::SetCurrentContext(m_pImPlotContext);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}

		int width{ }, height{ };
		BGUTGetWindowSize(BGUTGetWindowPtr(), width, height);

		D3D11_VIEWPORT viewport = { };
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<decltype(D3D11_VIEWPORT::Width)>(width);
		viewport.Height = static_cast<decltype(D3D11_VIEWPORT::Height)>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		const float kClearColor[] = { m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a };

		// Clear the render target
		m_pDeviceContext->ClearRenderTargetView(
			m_pRenderTargetView.Get(),
			kClearColor);

		// Set the viewport
		m_pDeviceContext->RSSetViewports(
			1u,
			&viewport);

		// Set the render targets
		m_pDeviceContext->OMSetRenderTargets(
			1u,
			m_pRenderTargetView.GetAddressOf(),
			nullptr);


		m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

		constexpr UINT kVERTEX_STRIDE = sizeof(VertexPositionColor);
		constexpr UINT kVERTEX_OFFSET = 0u;
		m_pDeviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &kVERTEX_STRIDE, &kVERTEX_OFFSET);

		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
		
		m_pDeviceContext->RSSetState(m_pSolidRasterState.Get());

		m_pDeviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);

		m_pDeviceContext->Draw(3u, 0u);
	}

	void D3D11Renderer::VEndFrame(void)
	{
		// When ImGui is enabled, call end of frame routines
		if (*m_options.bImGuiEnabled)
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		// Present the frame to the screen (1 enables v-sync)
		m_pSwapChain->Present((*m_options.bVSync ? 1U : 0U), 0U); 
	}

	StrongIShaderFactoryPtr D3D11Renderer::VCreateShaderFactory(void)
	{
		//return std::make_shared<GLShaderFactory>();
		return nullptr;
	}

	StrongIShaderProgramPtr D3D11Renderer::VCreateShaderProgram(std::string_view name)
	{
		//auto pShaderProgram = std::make_shared<GLShaderProgram>();
		//return pShaderProgram;
		return nullptr;
	}

	StrongIShaderProgramPtr D3D11Renderer::VGetShaderProgram(std::string_view name)
	{
		return nullptr;
	}

	void D3D11Renderer::VOnResize(std::int32_t width, std::int32_t height)
	{
		BGE_ASSERT(width >= 0 && height >= 0);

		m_pDeviceContext->Flush();

		DestroySwapchainResources();

		if (FAILED(m_pSwapChain->ResizeBuffers(
			0u,
			static_cast<UINT>(width),
			static_cast<UINT>(height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0u)))
		{
			BGE_ERROR("D3D11: Failed to recreate swap chain buffers");
			return;
		}

		if (!CreateSwapchainResources())
		{
			BGE_ERROR("Failed to create swap chain resources");
			return;
		}
	}

	void D3D11Renderer::VSetViewport(const IViewport &kViewport)
	{
		m_pViewport = std::make_unique<GLViewport>(dynamic_cast<const GLViewport &>(kViewport));
	}

	const IViewport &D3D11Renderer::VGetViewport(void) const
	{
		BGE_ASSERT(m_pViewport);
		return *m_pViewport.get();
	}

	void D3D11Renderer::VSetBackgroundColor(const glm::vec4 &kColor)
	{
		m_bgColor = kColor;
	}

	glm::vec4 D3D11Renderer::VGetBackgroundColor(void)
	{
		return m_bgColor;
	}

	void D3D11Renderer::VEnableDepthTest(bool bEnable)
	{
	}

	void D3D11Renderer::VEnableBlending(bool bEnable)
	{
	}

	void D3D11Renderer::VSetBlendMode(BlendMode mode)
	{
	}

	BlendMode D3D11Renderer::VGetBlendMode(void) const
	{
		return BlendMode();
	}

	bool D3D11Renderer::VTakeScreenshot(std::string_view saveGameDir)
	{
		BGE_ASSERT(m_pRenderTargetView);

		ID3D11Resource *pResource = nullptr;
		m_pRenderTargetView->GetResource(&pResource); // Query the associated resource

		ID3D11Texture2D *pRenderTargetTexture = nullptr;
		if (pResource)
		{
			// Cast the resource to ID3D11Texture2D
			pResource->QueryInterface(IID_PPV_ARGS(&pRenderTargetTexture));
			pResource->Release(); // Release the intermediate resource
		}

		if (!pRenderTargetTexture)
		{
			return false;
		}

		// Get the description of the render target texture
		D3D11_TEXTURE2D_DESC renderTargetDesc;
		pRenderTargetTexture->GetDesc(&renderTargetDesc);

		// Create a staging texture (CPU-readable)
		D3D11_TEXTURE2D_DESC stagingDesc = renderTargetDesc;
		stagingDesc.Usage = D3D11_USAGE_STAGING;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagingDesc.BindFlags = 0u; // No binding flags needed
		stagingDesc.MiscFlags = 0u;

		// TODO: Check the sample count of the render target to support screenshots of multisampled render target view.
		
		ID3D11Texture2D *pStagingTexture = nullptr;
		if (FAILED(m_pDevice->CreateTexture2D(&stagingDesc, nullptr, &pStagingTexture)))
		{
			BGE_LOG("D3D11", "Failed to create staging texture");
			pRenderTargetTexture->Release();
			return false;
		}

		// Copy render target texture to the staging texture
		m_pDeviceContext->CopyResource(pStagingTexture, pRenderTargetTexture);

		// Map the staging texture to access its data on the CPU
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(m_pDeviceContext->Map(pStagingTexture, 0u, D3D11_MAP_READ, 0u, &mappedResource)))
		{
			pStagingTexture->Release();
			return false;
		}

		// Get the raw data
		std::uint8_t *pData = reinterpret_cast<std::uint8_t *>(mappedResource.pData);

		SDL_Surface *pSurface = SDL_CreateRGBSurfaceWithFormatFrom(
			mappedResource.pData,
			renderTargetDesc.Width,
			renderTargetDesc.Height,
			(renderTargetDesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM ? 32 : 24), // Bits per pixel
			mappedResource.RowPitch,
			SDL_PIXELFORMAT_ARGB8888);

		if (!pSurface)
		{
			BGE_LOG("SDL", "Failed to create SDL_Surface: %s", SDL_GetError());
			m_pDeviceContext->Unmap(pStagingTexture, 0u);
			pStagingTexture->Release();
			pRenderTargetTexture->Release();
			return false;
		}

		// Save the surface to a BMP file
		std::string screenshotPath = GetScreenshotFilename(saveGameDir);
		if (SDL_SaveBMP(pSurface, screenshotPath.c_str()))
		{
			BGE_LOG("SDL", "Failed to save screenshot: %s", SDL_GetError());
		}

		// Clean up
		SDL_FreeSurface(pSurface);
		m_pDeviceContext->Unmap(pStagingTexture, 0u);
		pStagingTexture->Release();
		pRenderTargetTexture->Release();

		return true;
	}

	void D3D11Renderer::VEnableDebugOutput(bool bEnable)
	{
	}

	std::string D3D11Renderer::VGetRendererInfo(void) const
	{
		return std::string();
	}
	
	bool D3D11Renderer::CreateSwapchainResources(void)
	{
		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		if (FAILED(m_pSwapChain->GetBuffer(
			0u,
			IID_PPV_ARGS(&pBackBuffer))))
		{
			BGE_ERROR("D3D11: Failed to get back buffer form the swap chain");
			return false;
		}

		if (FAILED(m_pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&m_pRenderTargetView)))
		{
			BGE_ERROR("D3D11: Failed to create render target view from back buffer");
			return false;
		}

		return true;
	}

	void D3D11Renderer::DestroySwapchainResources(void)
	{
		m_pRenderTargetView.Reset();
	}

	bool D3D11Renderer::CompileShader(StrongResourceHandlePtr pResourceHandle, std::string_view entryPoint,
									  std::string_view profile, ComPtr<ID3DBlob> &pShaderBlob)
	{
		BGE_ASSERT(pResourceHandle->GetType() == ResourceType::kHLSL);

		constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		ComPtr<ID3DBlob> pTempShaderBlob = nullptr;
		ComPtr<ID3DBlob> pErrorBlob = nullptr;

		auto pExtraData = std::static_pointer_cast<PlaintextResourceExtraData>(pResourceHandle->GetExtraData());
		std::string shaderSource = pExtraData->VGetExtraData();
		
		HRESULT hr = D3DCompile(
			shaderSource.c_str(), shaderSource.size(),
			nullptr, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.data(), profile.data(),
			compileFlags, 0u,
			&pTempShaderBlob, &pErrorBlob);
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

		pShaderBlob = std::move(pTempShaderBlob);
		return true;
	}
	
	D3D11Renderer::ComPtr<ID3D11VertexShader> D3D11Renderer::CreateVertexShader(StrongResourceHandlePtr pResourceHandle, ComPtr<ID3DBlob> &pShaderBlob)
	{
		if (!CompileShader(pResourceHandle, "Main", "vs_5_0", pShaderBlob))
		{
			return nullptr;
		}

		ComPtr<ID3D11VertexShader> pVertexShader;
		HRESULT hr = m_pDevice->CreateVertexShader(
			pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
			nullptr,
			&pVertexShader);
		if (FAILED(hr))
		{
			BGE_ERROR("D3D11: Failed to compile vertex shader");
			return nullptr;
		}

		return pVertexShader;
	}
	
	D3D11Renderer::ComPtr<ID3D11PixelShader> D3D11Renderer::CreatePixelShader(StrongResourceHandlePtr pResourceHandle)
	{
		ComPtr<ID3DBlob> pShaderBlob = nullptr;
		if (!CompileShader(pResourceHandle, "Main", "ps_5_0", pShaderBlob))
		{
			return nullptr;
		}

		ComPtr<ID3D11PixelShader> pPixelShader;
		HRESULT hr = m_pDevice->CreatePixelShader(
			pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
			nullptr,
			&pPixelShader);
		if (FAILED(hr))
		{
			BGE_ERROR("D3D11: Failed to compile pixel shader");
			return nullptr;
		}

		return pPixelShader;
	}
} // End namespace (BGE)
