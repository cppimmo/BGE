/*=============================================================================*
 * BGEngine.cpp - GLUT-esque abstraction of SDL and app layer.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
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
#include "EngineStd.hpp"
#include "BGUT.hpp"

#include "Graphics/Debug.hpp"
#include "Utilities/Utils.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <cstdlib>

namespace BGE
{
	/**
	 * BGUTData manages the state of the utility toolkit.
	 */
	struct BGUTData
	{
		SDL_Window *pWindow = nullptr;
		SDL_WindowFlags defWindowFlags{};
		SDL_GLContext pContext = nullptr;
		struct OpenGLVersion
		{
			int major = 4, minor = 2;
		} glVersion;
		bool glDebugEnabled = false;
		std::string defWindowTitle = std::string();
		int defWindowWidth = 800;
		int defWindowHeight = 600;
		bool windowResizable = false;
		bool fullscreenEnabled = false;
		bool verticalSyncEnabled = false;
		int multisamplingLevel = 0;
		bool imGuiEnabled = false;
		bool isRunning = false;
		bool toLimitFrames = false;
		Uint32 minFrames = 6;
		Timer mainLoopTimer{};
		BGUTUpdateCallback pUpdateCallback = nullptr;
		BGUTRenderCallback pRenderCallback = nullptr;
		BGUTEventHandlerCallback pEventHandlerCallback = nullptr;
		int exitCode = BGE_EXIT_SUCCESS;
	};
	
	static bool BGUTParseConfig(std::string_view configFilename, BGUTData &data);
	static bool BGUTInitImGui(BGUTWindowPtr pWindow); // also for ImPlot
	static void BGUTShutdownImGui(void);
	static void BGUTLogInfo(void);
	static void BGUTSetAttributes(int versionMajor, int versionMinor, bool doubleBuffered, bool debugEnabled);
	static void BGUTDefEventHandler(const SDL_Event &event);
	
	static BGUTData s_BGUT = {};
} // End namespace (BGE)

bool BGE::BGUTInit(std::string_view configFilename)
{
	// Parse engine configuration file
	if (!BGUTParseConfig(configFilename, s_BGUT))
	{
		BGE_ERROR("BGUTInit Failure: Couldn't parse config file!");
		return false;
	}
	// Decide which parts of SDL should be initialized
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		BGE_ERROR("BGUTInit Failure: SDL failed to initialize (%s).", SDL_GetError());
		return false;
	}

	SDL_LogSetOutputFunction(Logger::LogOutputFunc_SDL, nullptr);
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	// Set OpenGL attributes before window creation
	BGUTSetAttributes(s_BGUT.glVersion.major, s_BGUT.glVersion.minor, true, s_BGUT.glDebugEnabled);
	// Set basic window flags
	s_BGUT.defWindowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	// When the window is set to be resizable
	if (s_BGUT.windowResizable && !s_BGUT.fullscreenEnabled)
	{
		s_BGUT.defWindowFlags = static_cast<SDL_WindowFlags>(s_BGUT.defWindowFlags | SDL_WINDOW_RESIZABLE);
	}
	// When the window is set to be fullscreen
	if (s_BGUT.fullscreenEnabled)
	{
		s_BGUT.defWindowFlags = static_cast<SDL_WindowFlags>(s_BGUT.defWindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
		// Retrive the current display mode:
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
		{
			BGE_ERROR("BGUTInit Failure: Could not retrieve current display mode (%s).", SDL_GetError());
			return false;
		}
		// Set the desired width and height of fullscreen window:
		s_BGUT.defWindowWidth = displayMode.w;
		s_BGUT.defWindowHeight = displayMode.h;
	}
	// Create the SDL window
	s_BGUT.pWindow = SDL_CreateWindow(s_BGUT.defWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									  s_BGUT.defWindowWidth, s_BGUT.defWindowHeight, s_BGUT.defWindowFlags);
	if (!s_BGUT.pWindow)
	{
		BGE_ERROR("BGUTInit Failure: SDL window could not be created (%s).", SDL_GetError());
		return false;
	}
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
	if (SDL_GL_SetSwapInterval((s_BGUT.verticalSyncEnabled) ? 1 : 0) < 0) // Vertical sync
	{
		BGE_ERROR("BGUTInit Failure: Can't set OpenGL swap interval (%s).", SDL_GetError());
		return false;
	}
	// Set the function loader for OpenGL
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		BGE_ERROR("BGUTInit Failure: glad OpenGL loader can't be set (%s).", SDL_GetError());
		return false;
	}
	// Perform extra setup for the OpenGL debug context
	if (s_BGUT.glDebugEnabled)
		GL::DebugContextSetup();

	// Only init ImGui when it is enabled (rely on short circuit evaluation)
	if (s_BGUT.imGuiEnabled && !BGUTInitImGui(s_BGUT.pWindow))
	{
		BGE_ERROR("BGUTInit Failure: Couldn't initialize ImGui!");
		return false;
	}
	// Set the OpenGL viewport
	BGUTSetViewport(0, 0, s_BGUT.defWindowWidth, s_BGUT.defWindowHeight);
	// Write some information to the log related to the toolkit
	BGUTLogInfo();
	return true;
}

bool BGE::BGUTCreateWindow(std::string_view windowTitle, std::string_view iconFilename)
{
	// Create the SDL window
	//s_BGUT.pWindow = SDL_CreateWindow(s_BGUT.defWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	//								  s_BGUT.defWindowWidth, s_BGUT.defWindowHeight, s_BGUT.defWindowFlags);
	return true;
}

void BGE::BGUTMainLoop(void)
{
	SDL_Event event; // Poll event
	s_BGUT.isRunning = true; // Set game running to true
	// https://gamedev.stackexchange.com/questions/151877/handling-variable-frame-rate-in-sdl2
	constexpr Uint64 kMillis = 1000;
	// Protect from divide by zero UB
	const Uint64 kTicksMinStepMillis = kMillis / ((s_BGUT.minFrames == 0) ? 1 : s_BGUT.minFrames); // Min delta
	Uint64 kTicksLastStepMillis = SDL_GetTicks64(); // Previous delta

	s_BGUT.mainLoopTimer.Start(); // Start the mainloop timer
	while (s_BGUT.isRunning) // Keep looping while isRunning is true
	{
		const Uint64 kTicksNowMillis = SDL_GetTicks64();
		while (SDL_PollEvent(&event))
		{
			// call default event handler
			BGUTDefEventHandler(event);
			// call user event handler callback
			if (s_BGUT.pEventHandlerCallback)
				s_BGUT.pEventHandlerCallback(event);
		}

		if (kTicksLastStepMillis < kTicksNowMillis)
		{
			Uint64 deltaTimeMS = kTicksNowMillis - kTicksLastStepMillis; // Delta time

			if (deltaTimeMS > kTicksMinStepMillis) // Set the current delta to the minimum
				deltaTimeMS = kTicksMinStepMillis;
			// Call update callback
			if (s_BGUT.pUpdateCallback)
				s_BGUT.pUpdateCallback(static_cast<float>(deltaTimeMS), s_BGUT.mainLoopTimer.GetElapsedMillis());

			kTicksLastStepMillis = kTicksNowMillis; // set previous step
			// when ImGui is enabled, prepare the new frame
			if (s_BGUT.imGuiEnabled)
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame();
				ImGui::NewFrame();
			}

			if (s_BGUT.pRenderCallback) // call render callback
				s_BGUT.pRenderCallback();
			// when ImGui is enabled, call end of frame routines
			if (s_BGUT.imGuiEnabled)
			{
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
		}
		else
		{
			if (s_BGUT.toLimitFrames) // limit frames if needed
				SDL_Delay(1u);
		}
		// swap OpenGL buffers on window
		SDL_GL_SwapWindow(s_BGUT.pWindow);
	}
	s_BGUT.mainLoopTimer.Stop(); // Stop the mainloop timer
}

void BGE::BGUTSendExitCode(int exitCode)
{
	s_BGUT.exitCode = exitCode; // set exit code for later retrieval
	s_BGUT.isRunning = false; // quit MainLoop
}

void BGE::BGUTShutdown(void)
{
	// when ImGui is enabled, shutdown its context
	if (s_BGUT.imGuiEnabled)
		BGUTShutdownImGui();

	SDL_GL_DeleteContext(s_BGUT.pContext);
	SDL_DestroyWindow(s_BGUT.pWindow);
	SDL_Quit();
}

void BGE::BGUTSetWindowFullscreen(BGUTWindowPtr pWindow, bool useFullscreen)
{

}

void BGE::BGUTSetWindowSize(BGUTWindowPtr pWindow, int width, int height)
{
	SDL_SetWindowSize(pWindow, width, height);
}

void BGE::BGUTSetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
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

SDL_Window *BGE::BGUTGetWindowPtr(void)
{
	return s_BGUT.pWindow;
}

SDL_GLContext BGE::BGUTGetContextPtr(void)
{
	return s_BGUT.pContext;
}

const BGE::Timer &BGE::BGUTGetMainLoopTimer(void)
{
	return s_BGUT.mainLoopTimer;
}

int BGE::BGUTGetExitCode(void)
{
	return s_BGUT.exitCode;
}

bool BGE::BGUTParseConfig(std::string_view configFilename, BGUTData &data)
{
	using namespace tinyxml2;
	XMLDocument xmlDocument; // Document object
	XMLError xmlResult; // Result object

	xmlResult = xmlDocument.LoadFile(configFilename.data());
	if (xmlResult != XML_SUCCESS)
	{
		BGE_ERROR("BGUTParseConfig Failure: Couldn't find config file!");
		return false;
	}
	// Fetch the root element: Engine
	auto *pRoot = xmlDocument.RootElement();
	if (!pRoot) return false;

	static constexpr const char *c_kpATTRIB_TAG_NAME = "name";
	static constexpr const char *c_kpATTRIB_VALUE_NAME = "value";
	for (auto *pElem = pRoot->FirstChildElement(); pElem; pElem->NextSiblingElement())
	{
		const std::string kOptionName(pElem->Attribute(c_kpATTRIB_TAG_NAME));
		// Look for known options
		if (kOptionName == "glVersion")
		{
			const int kMajor = pElem->IntAttribute("major");
			const int kMinor = pElem->IntAttribute("minor");
			s_BGUT.glVersion = { kMajor, kMinor };
		}
		else if (kOptionName == "glDebugEnabled")
		{
			const bool kValue = pElem->BoolAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.glDebugEnabled = kValue;
		}
		else if (kOptionName == "defWindowTitle")
		{
			const char *pkValue = pElem->Attribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.defWindowTitle = pkValue;
		}
		else if (kOptionName == "defWindowWidth")
		{
			const int kValue = pElem->IntAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.defWindowWidth = kValue;
		}
		else if (kOptionName == "defWindowHeight")
		{
			const int kValue = pElem->IntAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.defWindowHeight = kValue;
		}
		else if (kOptionName == "windowResizable")
		{
			const bool kValue = pElem->Attribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.windowResizable = kValue;
		}
		else if (kOptionName == "fullscreenEnabled")
		{
			const bool kValue = pElem->BoolAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.fullscreenEnabled = kValue;
		}
		else if (kOptionName == "verticalSyncEnabled")
		{
			const bool kValue = pElem->BoolAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.verticalSyncEnabled = kValue;
		}
		else if (kOptionName == "MSAA")
		{
			const int kValue = pElem->IntAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.multisamplingLevel = kValue;
		}
		else if (kOptionName == "imGuiEnabled")
		{
			const bool kValue = pElem->BoolAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.imGuiEnabled = kValue;
		}
		else if (kOptionName == "toLimitFrames")
		{
			const bool kValue = pElem->BoolAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.toLimitFrames = kValue;
		}
		else if (kOptionName == "minFrames")
		{
			const int kValue = pElem->IntAttribute(c_kpATTRIB_VALUE_NAME);
			s_BGUT.minFrames = kValue;
		}
	}
	return true;
}

bool BGE::BGUTInitImGui(BGUTWindowPtr pWindow)
{
	IMGUI_CHECKVERSION(); // What does this do?
	// Create ImGui context
	if (!ImGui::CreateContext())
	{
		BGE_ERROR("BGUTInitImGui Failure: Couldn't create ImGui context!");
		return false;
	}
	// Create ImPlot context
	if (!ImPlot::CreateContext())
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
	if (s_BGUT.imGuiEnabled)
		BGE_INFO("ImGui Version: %s", ImGui::GetVersion());

	BGE_INFO("Current OpenGL Version: %d.%d", s_BGUT.glVersion.major, s_BGUT.glVersion.minor);

	int attribValue = 0;
	static constexpr std::size_t numAttribs = 27 + 1;
	for (std::size_t index = 0; index < numAttribs; ++index)
	{
		SDL_GL_GetAttribute(static_cast<SDL_GLattr>(index), &attribValue);
		BGE_INFO("SDL_GLattr(%u): %d(0x%08X)", index, attribValue, attribValue);
	}

	BGE_INFO_IF(glGetString(GL_VENDOR) != 0, "GL_VENDOR: %s", glGetString(GL_VENDOR));
	BGE_INFO_IF(glGetString(GL_RENDERER) != 0, "GL_RENDERER: %s", glGetString(GL_RENDERER));
	BGE_INFO_IF(glGetString(GL_VERSION) != 0, "GL_VERSION: %s", glGetString(GL_VERSION));
	BGE_INFO_IF(glGetString(GL_SHADING_LANGUAGE_VERSION) != 0, "GL_SHADING_LANGUAGE: %s",
				glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void BGE::BGUTSetAttributes(int versionMajor, int versionMinor, bool doubleBuffered, bool debugEnabled)
{
	// Set OpenGL context attributes:
	SDL_GL_LoadLibrary(nullptr);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Use hardware 3D
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versionMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	if (debugEnabled)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffered);
	// Set multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, (s_BGUT.multisamplingLevel > 0) ? 1 : 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, s_BGUT.multisamplingLevel); // Set level
	//glEnable(GL_MULTISAMPLE);
}

void BGE::BGUTDefEventHandler(const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		s_BGUT.isRunning = false;
		break;
	case SDL_WINDOWEVENT:
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			// TODO: Should default size be updated?  Should be configured by renderer?
			BGUTSetViewport(0, 0, event.window.data1, event.window.data2);
			break;
		}
		break;
	}
	// Call ImGui event handler when enabled
	if (s_BGUT.imGuiEnabled)
		ImGui_ImplSDL2_ProcessEvent(&event);
}