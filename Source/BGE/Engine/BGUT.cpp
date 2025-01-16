/*******************************************************************************
 * @file   BGUT.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  GLUT-esque abstraction of SDL.
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
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
 ******************************************************************************/
#include "EngineStd.hpp"
#include "BGUT.hpp"

#include <cstdlib>

#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace BGE
{
	struct OpenGLVersion
	{
		int major, minor;
	};
	static constexpr OpenGLVersion kOPENGL_VERSION = { .major = 4, .minor = 5 };

	/**
	 * BGUTData manages the state of the utility toolkit.
	 */
	struct BGUTData
	{
		EngineOptions options;
		SDL_Window *pWindow = nullptr;
		SDL_GLContext pContext = nullptr;
		std::uint32_t windowFlags{};
		int windowWidth = 800;
		int windowHeight = 600;
		struct ImGuiContexts
		{
			ImGuiContext *pImGuiContext = nullptr;
			ImPlotContext *pImPlotContext = nullptr;
		} imGuiContexts;
		bool bRunning = false;
		bool bLimitFrames = false;
		Uint32 minFrames = 6;
		Timer mainLoopTimer{};
		BGUTUpdateCallback pUpdateCallback = nullptr;
		BGUTRenderCallback pRenderCallback = nullptr;
		BGUTEventHandlerCallback pEventHandlerCallback = nullptr;
		BGUTResizeCallback pResizeCallback = nullptr;
		int exitCode = kBGE_EXIT_SUCCESS;
	};
	
	static bool BGUTInitImGui(BGUTWindowPtr pWindow); // also for ImPlot
	static void BGUTShutdownImGui(void);
	static void BGUTLogInfo(void);
	static void BGUTSetAttributes(int versionMajor, int versionMinor, bool bDoubleBuffered, bool bDebugEnabled);
	static bool BGUTDefEventHandler(const SDL_Event &event);
	
	static BGUTData s_BGUT = {};
} // End namespace (BGE)

bool BGE::BGUTInit(const EngineOptions &kOptions)
{
	s_BGUT.options = kOptions;
	// Decide which parts of SDL should be initialized
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		BGE_ERROR("BGUTInit Failure: SDL failed to initialize (%s).", SDL_GetError());
		return false;
	}

	// Set SDL log output callback
	SDL_LogSetOutputFunction(Logger::LogOutputFunc_SDL, nullptr);
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

	//BGE_LOG("BGUT", "Request OpenGL version %d.%d", s_BGUT.glVersion.major, s_BGUT.glVersion.minor);
	// Set OpenGL attributes before window creation
	
	
	// Set basic window flags
	s_BGUT.windowFlags = SDL_WINDOW_SHOWN;
	if (*kOptions.rendererImpl == RendererImpl::kOpenGL)
	{
		s_BGUT.windowFlags |= SDL_WINDOW_OPENGL;
	}

	// When the window is set to be resizable
	if (*kOptions.bWindowResizable && !(*kOptions.bFullscreen))
	{
		s_BGUT.windowFlags |= SDL_WINDOW_RESIZABLE;
	}
	
	s_BGUT.windowWidth = *kOptions.windowWidth;
	s_BGUT.windowHeight = *kOptions.windowHeight;
	// When the window is set to be fullscreen
	if (*kOptions.bFullscreen)
	{
		s_BGUT.windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		// Retrive the current display mode:
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
		{
			BGE_ERROR("BGUTInit Failure: Could not retrieve current display mode (%s).", SDL_GetError());
			return false;
		}
		// Set the desired width and height of fullscreen window:
		s_BGUT.windowWidth = displayMode.w;
		s_BGUT.windowHeight = displayMode.h;
	}

	if (*kOptions.rendererImpl == RendererImpl::kOpenGL)
	{
		BGUTSetAttributes(kOPENGL_VERSION.major, kOPENGL_VERSION.minor, true, *kOptions.bRendererDebug);
	}

	// Create the SDL window
	s_BGUT.pWindow = SDL_CreateWindow((*kOptions.windowTitle).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									  s_BGUT.windowWidth, s_BGUT.windowHeight, s_BGUT.windowFlags);
	if (!s_BGUT.pWindow)
	{
		BGE_ERROR("BGUTInit Failure: SDL window could not be created (%s).", SDL_GetError());
		return false;
	}
	
	// Set the function loader for OpenGL
	if (*kOptions.rendererImpl == RendererImpl::kOpenGL)
	{
		// Attempt to create the OpenGL context
		s_BGUT.pContext = SDL_GL_CreateContext(s_BGUT.pWindow);
		if (!s_BGUT.pContext)
		{
			BGE_ERROR("BGUTInit Failure: OpenGL context could not be created (%s).", SDL_GetError());
			return false;
		}
	
		// Set the current OpenGL context
		if (SDL_GL_MakeCurrent(s_BGUT.pWindow, s_BGUT.pContext) < 0)
		{
			BGE_ERROR("BGUTInit Failure: OpenGL context could not be set (%s).", SDL_GetError());
			return false;
		}
	
		// Determine if vertical sync should be enabled
		if (SDL_GL_SetSwapInterval((kOptions.bVSync ? 1 : 0) < 0)) // Vertical sync
		{
			BGE_ERROR("BGUTInit Failure: Can't set OpenGL swap interval (%s).", SDL_GetError());
			return false;
		}

		const int kGladVersion = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
		if (kGladVersion == 0)
		{
			BGE_ERROR("BGUTInit Failure: glad OpenGL loader can't be set (%s).", SDL_GetError());
			return false;
		}
		const int kGladMajorVersion = GLAD_VERSION_MAJOR(kGladVersion);
		const int kGladMinorVersion = GLAD_VERSION_MINOR(kGladVersion);
		BGE_LOG("BGUT", "Loaded OpenGL %d.%d", kGladMajorVersion, kGladMinorVersion);
	}
	// TODO: This should go in the renderer implementation
	// Perform extra setup for the OpenGL debug context
	//if (s_BGUT.bGLDebugEnabled)
	//{
	//	GL::DebugContextSetup();
	//}
	
	// TODO: This should go in the renderer implementation
	// Only init ImGui when it is enabled (rely on short circuit evaluation)
	//if (s_BGUT.bImGuiEnabled && !BGUTInitImGui(s_BGUT.pWindow))
	//{
	//	BGE_ERROR("BGUTInit Failure: Couldn't initialize ImGui!");
	//	return false;
	//}
	
	// Let the app layer set the viewport
	//if (s_BGUT.pResizeCallback)
	//	s_BGUT.pResizeCallback(s_BGUT.windowWidth, s_BGUT.windowHeight);
	
	// Write some information to the log related to the toolkit
	BGUTLogInfo();
	
	return true;
}

bool BGE::BGUTCreateWindow(std::string_view windowTitle, std::string_view iconFilename)
{
	// Create the SDL window
	//s_BGUT.pWindow = SDL_CreateWindow(s_BGUT.defWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	//								  s_BGUT.windowWidth, s_BGUT.windowHeight, s_BGUT.windowFlags);
	return true;
}

void BGE::BGUTMainLoop(void)
{
	SDL_Event event; // Poll event
	s_BGUT.bRunning = true; // Set game running to true
	// https://gamedev.stackexchange.com/questions/151877/handling-variable-frame-rate-in-sdl2
	constexpr Uint64 kMILLIS = 1000;
	// Protect from divide by zero UB
	const Uint64 kTicksMinStepMillis = kMILLIS / ((s_BGUT.minFrames == 0) ? 1 : s_BGUT.minFrames); // Min delta
	Uint64 kTicksLastStepMillis = SDL_GetTicks64(); // Previous delta

	s_BGUT.mainLoopTimer.Start(); // Start the mainloop timer
	while (s_BGUT.bRunning) // Keep looping while bRunning is true
	{
		const Uint64 kTicksNowMillis = SDL_GetTicks64();
		while (SDL_PollEvent(&event))
		{
			// Call default event handler
			BGUTDefEventHandler(event);
			// Call user event handler callback
			// TODO: Do something with the result of the callback
			if (s_BGUT.pEventHandlerCallback)
				s_BGUT.pEventHandlerCallback(event);
		}

		// Sleep main loop if the window is minimized
		if (SDL_GetWindowFlags(s_BGUT.pWindow) & SDL_WINDOW_MINIMIZED)
		{
			SDL_Delay(1U);
			continue;
		}

		if (kTicksLastStepMillis < kTicksNowMillis)
		{
			Uint64 deltaTimeMS = kTicksNowMillis - kTicksLastStepMillis; // Delta time

			if (deltaTimeMS > kTicksMinStepMillis) // Set the current delta to the minimum
				deltaTimeMS = kTicksMinStepMillis;
			// Call update callback
			if (s_BGUT.pUpdateCallback)
				s_BGUT.pUpdateCallback(static_cast<float>(deltaTimeMS), s_BGUT.mainLoopTimer.GetElapsedSecs());

			kTicksLastStepMillis = kTicksNowMillis; // Set previous step
			
			// TODO: ImGui rendering should be placed in a different routine than the render callback
			// When ImGui is enabled, prepare the new frame
			//if (s_BGUT.bImGuiEnabled)
			//{
			//	ImGui::SetCurrentContext(s_BGUT.imGuiContexts.pImGuiContext);
			//	ImPlot::SetCurrentContext(s_BGUT.imGuiContexts.pImPlotContext);
			//
			//	ImGui_ImplOpenGL3_NewFrame();
			//	ImGui_ImplSDL2_NewFrame();
			//	ImGui::NewFrame();
			//}

			if (s_BGUT.pRenderCallback) // Call render callback
				s_BGUT.pRenderCallback(static_cast<float>(deltaTimeMS), s_BGUT.mainLoopTimer.GetElapsedSecs());
			// When ImGui is enabled, call end of frame routines
			//if (s_BGUT.bImGuiEnabled)
			//{
			//	ImGui::Render();
			//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			//}
		}
		else
		{
			if (s_BGUT.bLimitFrames) // Limit frames if needed
				SDL_Delay(1U);
		}
		// Swap OpenGL buffers on window
		if (*s_BGUT.options.rendererImpl == RendererImpl::kOpenGL)
		{
			SDL_GL_SwapWindow(s_BGUT.pWindow);
		}
	}
	s_BGUT.mainLoopTimer.Stop(); // Stop the mainloop timer
}

void BGE::BGUTSendExitCode(int exitCode)
{
	s_BGUT.exitCode = exitCode; // Set exit code for later retrieval
	s_BGUT.bRunning = false; // Quit MainLoop
}

void BGE::BGUTShutdown(void)
{
	// When ImGui is enabled, shutdown its context
	//if (s_BGUT.bImGuiEnabled)
	//{
	//	BGUTShutdownImGui();
	//}
	
	if (s_BGUT.options.rendererImpl == RendererImpl::kOpenGL)
	{
		SDL_GL_DeleteContext(s_BGUT.pContext);
		//gladLoaderUnloadGL(); // Unload glad
	}

	SDL_DestroyWindow(s_BGUT.pWindow);
	SDL_Quit();
}

void BGE::BGUTSetWindowTitle(std::string_view title)
{
	if (!s_BGUT.pWindow) return;
	SDL_SetWindowTitle(s_BGUT.pWindow, title.data());
}


void BGE::BGUTSetWindowFullscreen(BGUTWindowPtr pWindow, bool bUseFullscreen)
{
}

void BGE::BGUTSetWindowIcon(const std::filesystem::path &kFilePath)
{
	SDL_Surface *pIconSurface = SDL_LoadBMP(kFilePath.string().c_str());
	if (!pIconSurface)
	{
		BGE_WARNING("Could't set window icon!");
		return;
	}

	SDL_SetWindowIcon(s_BGUT.pWindow, pIconSurface);
	SDL_FreeSurface(pIconSurface);
}

void BGE::BGUTSetWindowSize(BGUTWindowPtr pWindow, int width, int height)
{
	SDL_SetWindowSize(pWindow, width, height);
}

void BGE::BGUTSetCallbackUpdate(BGUTUpdateCallback pUpdateCallback)
{
	s_BGUT.pUpdateCallback = pUpdateCallback;
}

void BGE::BGUTSetCallbackRender(BGUTRenderCallback pRenderCallback)
{
	s_BGUT.pRenderCallback = pRenderCallback;
}

void BGE::BGUTSetCallbackEventHandler(BGUTEventHandlerCallback pEventHandlerCallback)
{
	s_BGUT.pEventHandlerCallback = pEventHandlerCallback;
}

void BGE::BGUTSetCallbackResize(BGUTResizeCallback pResizeCallback)
{
	s_BGUT.pResizeCallback = pResizeCallback;
}

SDL_Window *BGE::BGUTGetWindowPtr(void)
{
	return s_BGUT.pWindow;
}

void BGE::BGUTGetWindowSize(BGUTWindowPtr pWindow, int &width, int &height)
{
	SDL_GetWindowSize(pWindow, &width, &height);
}

SDL_GLContext BGE::BGUTGetContextPtr(void)
{
	return s_BGUT.pContext;
}

void BGE::BGUTSetImGuiContextPtrs(ImGuiContext *pImGuiContext, ImPlotContext *pImPlotContext)
{
	s_BGUT.imGuiContexts.pImGuiContext = pImGuiContext;
	s_BGUT.imGuiContexts.pImPlotContext = pImPlotContext;
}

ImGuiContext *BGE::BGUTGetImGuiContextPtr(void)
{
	return s_BGUT.imGuiContexts.pImGuiContext;
}

ImPlotContext *BGE::BGUTGetImPlotContextPtr(void)
{
	return s_BGUT.imGuiContexts.pImPlotContext;
}

const BGE::Timer &BGE::BGUTGetMainLoopTimer(void)
{
	return s_BGUT.mainLoopTimer;
}

int BGE::BGUTGetExitCode(void)
{
	return s_BGUT.exitCode;
}

bool BGE::BGUTInitImGui(BGUTWindowPtr pWindow)
{
	IMGUI_CHECKVERSION(); // What does this do?
	// Create ImGui context
	if (!(s_BGUT.imGuiContexts.pImGuiContext = ImGui::CreateContext()))
	{
		BGE_ERROR("BGUTInitImGui Failure: Couldn't create ImGui context!");
		return false;
	}
	// Create ImPlot context
	if (!(s_BGUT.imGuiContexts.pImPlotContext = ImPlot::CreateContext()))
	{
		BGE_ERROR("BGUTInitImGui Failure: Couldn't create ImPlot context!");
		return false;
	}

	ImGuiIO &io = ImGui::GetIO();
	//io.IniFilename = nullptr; // TODO: Set ImGui config filename/location.
	// Set ImGui style colors
	ImGui::StyleColorsDark();
	// Setup platform/renderer backends
	if (!ImGui_ImplSDL2_InitForOpenGL(pWindow, SDL_GL_GetCurrentContext()))
	{
		BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize SDL2 implementation!");
		return false;
	}
	// TODO: Define the GLSL version string elsewhere.
	if (!ImGui_ImplOpenGL3_Init("#version 330"))
	{
		BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize OpenGL3 implementation!");
		return false;
	}
	return true;
}

void BGE::BGUTShutdownImGui(void)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImPlot::DestroyContext(); // Destroy ImPlot context first
	ImGui::DestroyContext();
}

void BGE::BGUTLogInfo(void)
{
	SDL_version version;
	SDL_GetVersion(&version);
	BGE_INFO("SDL Version: %d.%d.%d", version.major, version.minor, version.patch);
	BGE_INFO("SDL Revision: %s", SDL_GetRevision());

	// When ImGui is enabled, log the version
	if (*s_BGUT.options.bImGuiEnabled)
	{
		BGE_INFO("ImGui Version: %s", ImGui::GetVersion());
	}
}

void BGE::BGUTSetAttributes(int versionMajor, int versionMinor, bool bDoubleBuffered, bool bDebugEnabled)
{
	// Set OpenGL context attributes:
	//SDL_GL_LoadLibrary(nullptr);
	int contextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
	if (bDebugEnabled)
	{
		contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versionMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, bDoubleBuffered ? 1 : 0);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Use hardware 3D

	// Set multisampling
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, (s_BGUT.multisamplingLevel > 0) ? 1 : 0);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, s_BGUT.multisamplingLevel); // Set level
	//glEnable(GL_MULTISAMPLE);
}

bool BGE::BGUTDefEventHandler(const SDL_Event &kEvent)
{
	switch (kEvent.type)
	{
	case SDL_QUIT:
		s_BGUT.bRunning = false;
		return true;
	case SDL_WINDOWEVENT:
		switch (kEvent.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			if (s_BGUT.pResizeCallback)
				s_BGUT.pResizeCallback(kEvent.window.data1, kEvent.window.data2);
			break;
		}
		return true;
	}
	// Call ImGui event handler when enabled
	if (*s_BGUT.options.bImGuiEnabled)
	{
		if (ImGui_ImplSDL2_ProcessEvent(&kEvent)) return true;
	}
	return false;
}
