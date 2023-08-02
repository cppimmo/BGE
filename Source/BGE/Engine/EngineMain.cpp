/*=============================================================================*
 * EngineMain.cpp : Defines the engine entry point
 *
 * Part of the BGEngine Project
 *
 * (c) Copyright 2022 Brian Hoffpauir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser GPL v3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 * http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
 *
 * You should have received a copy of the GNU Lesser GPL v3
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
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

int BGE::EngineMain(int numArgs, char *pArgv[])
{
	const auto kArgsSpan = GetArguments(numArgs, pArgv);
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
	if (!Prepare())
	{
		BGE_ERROR("Engine preparation failure.");
		return 1;
	}

	// Initialize logging system
	Logger::Init("Logging.xml");
	// Try to initialize the utility toolkit
	if (!BGUTInit("Engine.xml"))
	{
		BGE_ERROR("Couldn't initialize engine!");
		return BGE_EXIT_FAILURE;
	}
	// Set the utility callbacks
	//BGUTSetCallbackUpdate(EngineApp::OnUpdate);
	//BGUTSetCallbackRender(EngineApp::OnRender);
	//BGUTSetCallbackEventHandler(EngineApp::OnHandleEvent);
	// Initialize an instance of the application layer
	//if (!g_pApp->VInitInstance())
	//{
	//	BGE_ERROR("Failure to initialize instance of application!");
	//	return BGE_EXIT_FAILURE;
	//}

	Init();
	// TODO: Use SDL_Set/GetWindowData to set class object pointer.
	BGUTSetCallbackUpdate(Update);
	BGUTSetCallbackRender(Render);
	BGUTSetCallbackEventHandler(HandleEvent);
	BGUTMainLoop(); // Enter main loop
	BGE_INFO("Main loop duration: %.2f seconds", BGUTGetMainLoopTimer().GetElapsedSecs());
	Shutdown(); // App shutdown
	
	BGUTShutdown(); // Shutdown upon exit of main loop
	
	// Destroy the logging system
	Logger::Destroy();
#if BGE_PLATFORM_WINDBG
	_CrtDumpMemoryLeaks(); // Report leaks to log
	std::cout << "Press enter to exit.\n";
	std::cin.get(); // Wait for enter key, so any leaks can be seen.
#endif /* BGE_PLATFORM_WINDBG */
	return BGUTGetExitCode(); // Return app exit code
	// return g_pApp->GetExitCode(); // Return app exit code
}

void DebugDumpClient(void *pUserPortion, std::size_t blockSize)
{
	std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pUserPortion);
	// Use cstdio since Logger will be destroyed.
	std::fprintf(stderr, "Memory leak at: %llu, bytes allocated: %llu", address, blockSize);
}

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
	
	const auto kSaveGameDir = GetSaveGameDirectory("cppimmo", "Tank Battles");
	// check for null optional
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
