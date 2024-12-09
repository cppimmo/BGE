/*=============================================================================*
 * EngineMain.cpp - Defines the engine entry point.
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
#include "Engine/EngineStd.hpp"
#include "Graphics/Screenshot.hpp"

#include <csignal>
#include <iostream>

#if BGE_PLATFORM_WINDBG
#include <crtdbg.h>
#endif

using namespace BGE;

static void DebugDumpClient(void *pUserPortion, std::size_t blockSize);

#if 0
static bool Prepare(void);
static bool Init(void);
static void Update(float deltaTime, float elapsedTime);
static void Render(void);
static void HandleEvent(const SDL_Event &event);
static void Shutdown(void);

static GLuint s_triangleVAO, s_triangleVBO;
static constexpr GLuint s_kNUM_VERTICES = 3;
static GLuint s_vertexShaderID, s_fragmentShaderID, s_programID;
static std::string s_saveGameDir;
static constexpr const char *s_pkVERTEX_SHADER_SOURCE = R"vs(
#version 420 compatibility

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 g_vertexColor;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0);
	g_vertexColor = inColor;
}
)vs";
static constexpr const char *s_pkFRAGMENT_SHADER_SOURCE = R"fs(
#version 420 compatibility

layout (location = 0) out vec4 outColor;

in vec3 g_vertexColor;

void main(void)
{
	outColor = vec4(g_vertexColor, 1.0);
}
)fs";
#endif

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
	const auto kArgsSpan = GetArguments(numArgs, pArgs); // Collect command line args
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

	// Set the utility callbacks to the static member functions
	BGUTSetCallbackUpdate(EngineApp::OnUpdate);
	BGUTSetCallbackRender(EngineApp::OnRender);
	BGUTSetCallbackEventHandler(EngineApp::OnHandleEvent);

	BGE_INFO("Initializing engine...");
	auto &app = GetEngineApp();
	// Initialize an instance of the application layer (also initializes BGUT)
	if (!app.VInitInstance())
	{
		BGE_ERROR("Failure to initialize instance of application!");
		return BGE_EXIT_FAILURE;
	}

	// TODO: Use SDL_Set/GetWindowData to set class object pointer.
	BGUTMainLoop(); // Enter main loop
	BGE_INFO("Main loop duration: %.2f seconds", BGUTGetMainLoopTimer().GetElapsedSecs());
	
	BGE_INFO("Shutting down engine...");
	BGUTShutdown(); // Shutdown upon exit of main loop
	// Destroy the logging system
	Logger::Destroy();
#if BGE_PLATFORM_WINDBG
	_CrtDumpMemoryLeaks(); // Report leaks to log
	std::cout << "Press enter to exit.\n";
	std::cin.get(); // Wait for enter key, so any leaks can be seen.
#endif /* BGE_PLATFORM_WINDBG */
	return app.GetExitCode(); // Return app exit code
}

void DebugDumpClient(void *pUserPortion, std::size_t blockSize)
{
	std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pUserPortion);
	// Use cstdio since Logger will be destroyed.
	std::fprintf(stderr, "Memory leak at: %llu, bytes allocated: %llu", address, blockSize);
}

#if 0
bool Prepare(void)
{
	HideConsole(); // TODO: This should be called by Logger based on configuration.
	BGE_INFO("Platform: %s", GetPlatform().data());
	BGE_INFO("CPU speed: %dMHz", ReadCPUSpeed());
	BGE_INFO("Logical CPU cores: %d", ReadLogicalCPUCores());
	if (!IsDiskSpaceAvailable(1'000))
	{
		BGE_ERROR("Not enough storage!");
		return false;
	}
	else
	{
		BGE_INFO("Adequate storage is available.");
	}
	
	if (!IsMemoryAvailable(1'000))
	{
		BGE_ERROR("Not enough memory!");
		return false;
	}
	else
	{
		BGE_INFO("Adequate memory is available.");
	}
	
	const auto kSaveGameDir = GetSaveGameDirectory("cppimmo", "TestGame");
	// Check for null optional
	if (!kSaveGameDir)
	{
		BGE_ERROR("Could not fetch save game directory!");
		return false;
	}
	else
	{
		BGE_INFO("Save game directory: %s", (*kSaveGameDir).c_str());
		s_saveGameDir = *kSaveGameDir;
	}

	if (!IsOnlyInstance("TestGame"))
	{
		BGE_WARNING("Not the only instance of the game.");
	}
	return true;
}

bool Init(void)
{
	static constexpr GLfloat vertices[s_kNUM_VERTICES][3 + 3] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
		{  0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
		{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f }
	};
	
	//glGenBuffers(1, &triangleVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glCreateBuffers(1, &s_triangleVBO);
	glNamedBufferStorage(s_triangleVBO, sizeof(vertices), vertices, 0);

	s_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLint vertexShaderSourceLen = std::strlen(s_pkVERTEX_SHADER_SOURCE);
	glShaderSource(s_vertexShaderID, 1, &s_pkVERTEX_SHADER_SOURCE, &vertexShaderSourceLen);
	glCompileShader(s_vertexShaderID);

	char msgBuff[256];
	glGetShaderInfoLog(s_vertexShaderID, 256, nullptr, msgBuff);
	msgBuff[255] = '\0';
	BGE_INFO("VERTEX SHADER: %s", msgBuff);

	s_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLint fragmentShaderSourceLen = std::strlen(s_pkFRAGMENT_SHADER_SOURCE);
	glShaderSource(s_fragmentShaderID, 1, &s_pkFRAGMENT_SHADER_SOURCE, &fragmentShaderSourceLen);
	glCompileShader(s_fragmentShaderID);

	glGetShaderInfoLog(s_fragmentShaderID, 256, nullptr, msgBuff);
	msgBuff[255] = '\0';
	BGE_INFO("FRAGMENT SHADER: %s", msgBuff);

	s_programID = glCreateProgram();
	glAttachShader(s_programID, s_vertexShaderID);
	glAttachShader(s_programID, s_fragmentShaderID);

	glLinkProgram(s_programID);
	glUseProgram(s_programID);
	// Use createvertexarrays
	glCreateVertexArrays(1, &s_triangleVAO);
	glBindVertexArray(s_triangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, s_triangleVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return true;
}

void Update(float deltaTime, float elapsedTime)
{
	static bool c_initialized = false;
	if (!c_initialized)
	{
		//glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0xDEADBEEF,
		//					 GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Howdy!");

		c_initialized = true;
	}
}

void Render(void)
{
	constexpr float kCLEAR_COLOR[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, kCLEAR_COLOR);

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();

	glBindVertexArray(s_triangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, s_kNUM_VERTICES);
}

void HandleEvent(const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			BGUTSendExitCode(BGE_EXIT_SUCCESS);
		if (event.key.keysym.sym == SDLK_s)
		{
			static bool c_initialized = false;
			if (!c_initialized)
			{
				TakeScreenshot(s_saveGameDir);
				BGE_INFO("Tried to take screenshot!");
				c_initialized = true;
			}
		}
		break;
	}
}

void Shutdown(void)
{
	glDeleteBuffers(1, &s_triangleVBO);
	glDeleteBuffers(1, &s_triangleVAO);
	glDeleteShader(s_vertexShaderID);
	glDeleteShader(s_fragmentShaderID);
	glDeleteProgram(s_programID);
}
#endif
