/*******************************************************************************
 * @file   EngineMain.cpp
 * @author Brian Hoffpauir
 * @date   12.29.2023
 * @brief  Defines the engine entry point.
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
#include "Engine/EngineStd.hpp"

#include "Utilities/Exception.hpp"
#include "MainLoop/CommandParser.hpp"

#include <cstddef>
#include <cstdio>
#include <csignal>
#include <iostream>
#include <sstream>

#if BGE_PLATFORM_WINDBG
#include <crtdbg.h>
#endif

using namespace BGE;

namespace // Declare static functions
{
	void DebugDumpClient(void *pUserPortion, std::size_t blockSize);
	void PrintVersion(void);
	void PrintUsage(void);
	void ParseArguments(std::span<std::string_view> arguments);
	void AtExit(void);
} // End namespace

/**
 * @brief Entry point for the engine application.
 *
 * Initializes the logging system, application layer, and the utility callbacks.
 * Starts the main loop and manages cleanup and shutdown processes upon exit.
 *
 * @param numArgs The number of command-line arguments.
 * @param pArgs The array of command-line arguments.
 * @return The application exit code, where 0 indicates success.
 */
int BGE::EngineMain(int numArgs, char *pArgs[])
{
	// Parse CLI arguments
	ParseArguments(GetArguments(numArgs, pArgs));

#if BGE_PLATFORM_WINDBG
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Retrieve the current flags
	// Don't actually free the blocks
	tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	// Perform memory check for each alloc/dealloc (this really slows the app)
	// tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	// Check for leaks at program exit
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	_CrtSetDumpClient(DebugDumpClient);
#endif /* BGE_PLATFORM_WINDBG */

	// Initialize logging system (needs to be done first)
	Logger::Init("Logging.xml");

	int retCode = kBGE_EXIT_FAILURE; // Assume failure as app exit will overwrite
	/*
	 * NOTE: This exception handler is for uncaught exceptions, the engine typically does
	 * not use exceptions anywhere else. Any exceptions caught here will likely be from
	 * 3rd party code.
	 */
	try
	{
		// Set the utility callbacks to the static member functions
		BGUTSetCallbackUpdate(EngineApp::OnUpdate);
		BGUTSetCallbackRender(EngineApp::OnRender);
		BGUTSetCallbackEventHandler(EngineApp::OnHandleEvent);

		BGE_INFO("Welcome to %s (%s) %s", kENGINE_ABBREV.data(), kENGINE_NAME.data(), kVERSION.VToString().c_str());
		BGE_INFO("Initializing engine...");

		auto &app = GetEngineApp();

		// Set signal handlers
		std::signal(SIGABRT, EngineApp::OnHandleSignal);
		std::signal(SIGFPE,  EngineApp::OnHandleSignal);
		std::signal(SIGILL,  EngineApp::OnHandleSignal);
		std::signal(SIGINT,  EngineApp::OnHandleSignal);
		std::signal(SIGSEGV, EngineApp::OnHandleSignal);
		std::signal(SIGTERM, EngineApp::OnHandleSignal);
		std::atexit(AtExit);

		// Initialize an instance of the application layer (also initializes BGUT)
		if (!app.VInitInstance())
		{
			BGE_ERROR("Failure to initialize instance of application!");
			return kBGE_EXIT_FAILURE;
		}
		// TODO: Use SDL_Set/GetWindowData to set class object pointer.
		BGUTMainLoop(); // Enter main loop

		BGE_INFO("Shutting down engine...");
		BGUTShutdown(); // Shutdown upon exit of main loop

		retCode = app.GetExitCode();
	}
	catch (const Exception &ex) // Handle subtypes of custom exception
	{
		BGE_ERROR("Unhandled exception caught at entry point: %s", ex.VWhat());
		return kBGE_EXIT_FAILURE;
	}
	catch (const std::exception &ex) // Handle std::exception's
	{
		BGE_ERROR("Unhandled exception caught at entry point: %s", ex.what());
		return kBGE_EXIT_FAILURE;
	}
	catch (...) // Handle unknown exceptions
	{
		BGE_ERROR("Unknown exception caught at entry point");
		return kBGE_EXIT_FAILURE;
	}

#if BGE_PLATFORM_WINDBG
	_CrtDumpMemoryLeaks(); // Report leaks to log
	std::cout << "Press enter to exit.\n";
	std::cin.get(); // Wait for enter key, so any leaks can be seen.
#endif /* BGE_PLATFORM_WINDBG */

	return retCode; // Return app exit code
}

namespace // Define static functions
{
	void DebugDumpClient(void *pUserPortion, std::size_t blockSize)
	{
		std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pUserPortion);
		// Use cstdio since Logger will be destroyed.
		std::fprintf(stderr, "Memory leak at: %llu, bytes allocated: %llu", address, blockSize);
	}

	void PrintVersion(void)
	{
		std::cout << kENGINE_ABBREV << " (" << kENGINE_NAME << ") " << kVERSION.VToString() << '\n';
	}

	void PrintUsage(void)
	{
		PrintVersion();
		// NOTE: Most of the options are just made up for now.
		std::cout << R"usage(
Usage: game [OPTION...]

General:
  --help                  Display this help message.
  --version               Show engine version and build information.

Configuration:
  --config=FILE           Use a custom configuration file.
  --reset-settings        Reset to default engine settings.

Graphics:
  --renderer=ENGINE       Select rendering engine (OpenGL, Vulkan).
  --resolution=WxH        Set screen resolution (e.g., 1920x1080).
  --fullscreen=ON|OFF     Toggle fullscreen mode.
  --vsync=ON|OFF          Enable or disable VSync.
  --fps-limit=FPS         Limit frame rate (e.g., 60).

Audio:
  --volume=LEVEL          Set audio volume (0-100).
  --mute                  Mute all audio.

Debugging:
  --log-level=LEVEL       Set logging level (debug, info, error).
  --debug-mode            Enable additional debug information.
  --profiling             Enable performance profiling.

Gameplay:
  --skip-intro            Skip introductory scenes.
  --level=NAME            Load a specific level.
  --savegame=FILE         Load save file.

Networking:
  --host=IP               Host the game server at the specified IP.
  --join=ADDRESS          Join a game at the specified address.

For detailed information about each option, refer to the documentation.
)usage";
	}

	void ParseArguments(std::span<std::string_view> arguments)
	{
		// Collect command line args
		CommandParser parser(arguments);

		if (parser.Boolean("help"))
		{
			PrintUsage();
			std::exit(kBGE_EXIT_SUCCESS);
		}

		if (parser.Boolean("version"))
		{
			PrintVersion();
			std::exit(kBGE_EXIT_SUCCESS);
		}
		// TODO: Remove this test code.
		int value = 0;
		if (parser.Integer("number", value))
		{
			std::cout << "Number: " << value << '\n';
		}

		std::string str;
		if (parser.String("test", str))
		{
			std::cout << "Test: " << str << '\n';
		}
	}

	void AtExit(void)
	{
		auto &app = GetEngineApp();
		app.OnShutdown();

		// Destroy the logging system
		Logger::Destroy();
	}
} // End namespace
