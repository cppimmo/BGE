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

#include "Graphics/D3D11/Viewport.hpp"

namespace BGE
{
	D3D11Renderer::D3D11Renderer(void)
		: m_options{},
		  m_pViewport(std::make_unique<D3D11Viewport>(glm::ivec2(1280, 720))),
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
		constexpr std::array<D3D_FEATURE_LEVEL, 2> kDEVICE_FEATURE_LEVELS = 
		{
			D3D_FEATURE_LEVEL_11_1,
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

#ifdef BGE_CONFIG_DEBUG
		if (*m_options.bRendererDebug && FAILED(m_pDevice.As(&m_pDebug)))
		{
			BGE_ERROR("D3D11: Failed to get the debug layer from the device");
			return false;
		}

		BGE_LOG("D3D11", "\n\n%s", VGetRendererInfo().c_str());
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

#if 0
		// Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		ComPtr<ID3D11DepthStencilState> pDepthStencilState;
		m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

		m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC depthStencilTextureDesc = { };
		depthStencilTextureDesc.Width = width;
		depthStencilTextureDesc.Height = height;
		depthStencilTextureDesc.MipLevels = 1u;
		depthStencilTextureDesc.ArraySize = 1u;
		depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilTextureDesc.SampleDesc.Count = 1u;
		depthStencilTextureDesc.SampleDesc.Quality = 0u;
		depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_pDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, &pDepthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0u;

		m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthTarget);

		m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), m_pDepthTarget.Get());
#endif

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

		// Initialize ImGui & ImPlot
		IMGUI_CHECKVERSION(); // What does this do?

		// Create ImGui context
		if (!(m_pImGuiContext = ImGui::CreateContext()))
		{
			BGE_ERROR("D3D11: Couldn't create ImGui context!");
			return false;
		}

		// Create ImPlot context
		if (!(m_pImPlotContext = ImPlot::CreateContext()))
		{
			BGE_ERROR("D3D11: Couldn't create ImPlot context!");
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

		auto &app = GetEngineApp();
		auto &resCache = app.GetResourceCache();
		ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
		m_pVertexShader = CreateVertexShader(resCache.GetHandle(Resource("Assets\\Shaders\\Test.vs.hlsl")), pVertexShaderBlob);
		if (!m_pVertexShader)
		{
			return false;
		}

		m_pPixelShader = CreatePixelShader(resCache.GetHandle(Resource("Assets\\Shaders\\Test.ps.hlsl")));
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

		// Set the default viewport
		VSetViewport(D3D11Viewport(glm::ivec2(width, height)));

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

		// Clear the render target
		const float kClearColor[] = { m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a };
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), kClearColor);

		//m_pDeviceContext->ClearDepthStencilView(m_pDepthTarget.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

		// Setup the viewport
		D3D11_VIEWPORT viewport = { };
		viewport.TopLeftX = static_cast<decltype(D3D11_VIEWPORT::TopLeftX)>(m_pViewport->VGetOffset().x);
		viewport.TopLeftY = static_cast<decltype(D3D11_VIEWPORT::TopLeftX)>(m_pViewport->VGetOffset().y);
		viewport.Width = static_cast<decltype(D3D11_VIEWPORT::Width)>(m_pViewport->VGetSize().x);
		viewport.Height = static_cast<decltype(D3D11_VIEWPORT::Height)>(m_pViewport->VGetSize().y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		//D3D11Viewport::GetD3D11Viewport(m_pViewport.get());

		m_pDeviceContext->RSSetViewports(1u, &viewport);

		// Set the render targets
		m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), nullptr);

		m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

		constexpr UINT kVERTEX_STRIDE = sizeof(VertexPositionColor);
		constexpr UINT kVERTEX_OFFSET = 0u;
		m_pDeviceContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &kVERTEX_STRIDE, &kVERTEX_OFFSET);

		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pDeviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
		
		// Scissor test
		//D3D11_RASTERIZER_DESC rasterDesc = { };
		//rasterDesc.FillMode = D3D11_FILL_SOLID;
		//rasterDesc.CullMode = D3D11_CULL_BACK;
		//rasterDesc.ScissorEnable = true;
		//ComPtr<ID3D11RasterizerState> pState;
		//m_pDevice->CreateRasterizerState(&rasterDesc, &pState);
		//D3D11_RECT rect = { 100, 100, 600, 600 };
		//m_pDeviceContext->RSSetScissorRects(1u, &rect);
		//m_pDeviceContext->RSSetState(pState.Get());

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
		m_pViewport = std::make_unique<D3D11Viewport>(dynamic_cast<const D3D11Viewport &>(kViewport));
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

	bool D3D11Renderer::VTakeScreenshot(const std::filesystem::path &kSaveGameDir)
	{
		BGE_ASSERT(m_pRenderTargetView);

		// Query the resource associated with the render target view
		ComPtr<ID3D11Resource> pResource = nullptr;
		m_pRenderTargetView->GetResource(&pResource);

		ComPtr<ID3D11Texture2D> pRenderTargetTexture = nullptr;
		if (pResource)
		{
			pResource.As(&pRenderTargetTexture); // Cast the resource to ID3D11Texture2D
		}

		if (!pRenderTargetTexture)
		{
			return false;
		}

		// Get the description of the render target texture
		D3D11_TEXTURE2D_DESC renderTargetDesc;
		pRenderTargetTexture->GetDesc(&renderTargetDesc);

		// Multisampled render targets need to be resolved before staging
		ComPtr<ID3D11Texture2D> pResolvedTexture = nullptr;
		if (renderTargetDesc.SampleDesc.Count > 1)
		{
			D3D11_TEXTURE2D_DESC resolveDesc = renderTargetDesc;
			resolveDesc.SampleDesc.Count = 1u; // Disable multisampling
			resolveDesc.SampleDesc.Quality = 0u;
			resolveDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage for the intermediate texture
			resolveDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // Allows resolution
			resolveDesc.CPUAccessFlags = 0u; // No CPU access needed
			resolveDesc.MiscFlags = 0u;

			if (FAILED(m_pDevice->CreateTexture2D(&resolveDesc, nullptr, &pResolvedTexture)))
			{
				BGE_LOG("D3D11", "Failed to create resolved texture");
				pResolvedTexture->Release();
				return false;
			}

			// Resolve the multisampled render target
			m_pDeviceContext->ResolveSubresource(pResolvedTexture.Get(), 0u, pRenderTargetTexture.Get(), 0u, renderTargetDesc.Format);

			// Replace the render target texture with the resolved texture
			pRenderTargetTexture = pResolvedTexture;
		}

		// Create a staging texture (CPU-readable)
		D3D11_TEXTURE2D_DESC stagingDesc = renderTargetDesc;
		stagingDesc.SampleDesc.Count = 1u;
		stagingDesc.SampleDesc.Quality = 0u;
		stagingDesc.Usage = D3D11_USAGE_STAGING;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagingDesc.BindFlags = 0u; // No binding flags needed
		stagingDesc.MiscFlags = 0u;

		ComPtr<ID3D11Texture2D> pStagingTexture = nullptr;
		if (FAILED(m_pDevice->CreateTexture2D(&stagingDesc, nullptr, &pStagingTexture)))
		{
			BGE_LOG("D3D11", "Failed to create staging texture");
			return false;
		}

		// Copy render target texture to the staging texture
		m_pDeviceContext->CopyResource(pStagingTexture.Get(), pRenderTargetTexture.Get());

		// Map the staging texture to access its data on the CPU
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(m_pDeviceContext->Map(pStagingTexture.Get(), 0u, D3D11_MAP_READ, 0u, &mappedResource)))
		{
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
			m_pDeviceContext->Unmap(pStagingTexture.Get(), 0u);
			return false;
		}

		// Save the surface to a BMP file
		std::string screenshotPath = GetScreenshotFilename(kSaveGameDir.string());
		if (SDL_SaveBMP(pSurface, screenshotPath.c_str()))
		{
			BGE_LOG("SDL", "Failed to save screenshot: %s", SDL_GetError());
		}

		// Clean up
		SDL_FreeSurface(pSurface);
		m_pDeviceContext->Unmap(pStagingTexture.Get(), 0u);
		return true;
	}

	void D3D11Renderer::VEnableDebugOutput(bool bEnable)
	{
	}

	std::string D3D11Renderer::VGetRendererInfo(void) const
	{
		BGE_ASSERT(m_pDevice && m_pDeviceContext);
		HRESULT hr = S_OK;

		// Retrieve and output the feature level of the device
		const D3D_FEATURE_LEVEL kFeatureLevel = m_pDevice->GetFeatureLevel();
		std::string featureLevelStr;
		switch (kFeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
			featureLevelStr = "11.1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
			featureLevelStr = "11.0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			featureLevelStr = "10.1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			featureLevelStr = "10.0";
			break;
		case D3D_FEATURE_LEVEL_9_3:
			featureLevelStr = "9.3";
			break;
		case D3D_FEATURE_LEVEL_9_2:
			featureLevelStr = "9.2";
			break;
		case D3D_FEATURE_LEVEL_9_1:
			featureLevelStr = "9.1";
			break;
		default:
			featureLevelStr = "Unknown";
			break;
		}

		std::ostringstream oss;
		oss << "Device feature level: " << featureLevelStr << '\n';

		// Output information of the device's chosen adapter
		ComPtr<IDXGIDevice1> pDXGIDevice = nullptr;
		hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));
		if (FAILED(hr))
		{
			return "";
		}

		ComPtr<IDXGIAdapter> pAdapter = nullptr;
		hr = pDXGIDevice->GetAdapter(&pAdapter);
		if (FAILED(hr))
		{
			return "";
		}

		if (pAdapter)
		{
			DXGI_ADAPTER_DESC adapterDesc = { };
			pAdapter->GetDesc(&adapterDesc);

			constexpr auto toMiB = [](std::size_t bytes) -> std::size_t
			{
				return bytes / (1024 * 1024);
			};

			constexpr std::string_view kMEBIBYTE = "MiB";
			oss << "Adapter desc: " << WStringToString(adapterDesc.Description) << '\n';
			oss << "Adapter dedicated video mem: " << toMiB(adapterDesc.DedicatedVideoMemory) << ' ' << kMEBIBYTE << '\n';
			oss << "Adapter dedicated system mem: " << toMiB(adapterDesc.DedicatedSystemMemory) << ' ' << kMEBIBYTE << '\n';
			oss << "Adapter shader system mem: " << toMiB(adapterDesc.SharedSystemMemory) << ' ' << kMEBIBYTE << '\n';
		}

		return oss.str();
	}
	
	ID3D11Device *D3D11Renderer::GetDevice(void) noexcept
	{
		auto &app = GetEngineApp(); // Retrieve the engine app (contains renderer instance)
		auto &renderer = dynamic_cast<D3D11Renderer &>(app.GetRenderer()); // Cast to derived type

		return renderer.m_pDevice.Get();
	}

	ID3D11DeviceContext *D3D11Renderer::GetDeviceContext(void) noexcept
	{
		auto &app = GetEngineApp(); // Retrieve the engine app (contains renderer instance)
		auto &renderer = dynamic_cast<D3D11Renderer &>(app.GetRenderer()); // Cast to derived type

		return renderer.m_pDeviceContext.Get();
	}

	std::vector<D3D11Renderer::ComPtr<IDXGIAdapter1>> D3D11Renderer::EnumerateAdapters(void)
	{
		std::vector<ComPtr<IDXGIAdapter1>> adapters;

		ComPtr<IDXGIAdapter1> pAdapter = nullptr;
		for (UINT i = 0; m_pDXGIFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(pAdapter);
		}

		return adapters;
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
