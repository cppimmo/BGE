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
		sampleDesc.Count = 1U;
		sampleDesc.Quality = 0U;

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
		swapChainDesc.BufferCount = 2;
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
			1U,
			&viewport);

		// Set the render targets
		m_pDeviceContext->OMSetRenderTargets(
			1U,
			m_pRenderTargetView.GetAddressOf(),
			nullptr);
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
			0U,
			static_cast<UINT>(width),
			static_cast<UINT>(height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0U)))
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
		stagingDesc.BindFlags = 0U; // No binding flags needed
		stagingDesc.MiscFlags = 0U;

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
		if (FAILED(m_pDeviceContext->Map(pStagingTexture, 0U, D3D11_MAP_READ, 0U, &mappedResource)))
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
			renderTargetDesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM ? 32 : 24, // Bits per pixel
			mappedResource.RowPitch,
			SDL_PIXELFORMAT_ARGB8888);

		if (!pSurface)
		{
			BGE_LOG("SDL", "Failed to create SDL_Surface: %s", SDL_GetError());
			m_pDeviceContext->Unmap(pStagingTexture, 0U);
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
		m_pDeviceContext->Unmap(pStagingTexture, 0U);
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
			0U,
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
} // End namespace (BGE)
