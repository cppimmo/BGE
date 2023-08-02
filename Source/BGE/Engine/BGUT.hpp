/*=============================================================================*
 * BGEngine.hpp - GLUT-esque abstraction of SDL.
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
#ifndef _BGE_BGUT_HPP_
#define _BGE_BGUT_HPP_

#include "Engine/EngineStd.hpp"

namespace BGE
{
	// 1st Arg (delta time milliseconds), 2nd Arg (elapsed time milliseconds)
	using BGUTUpdateCallback = std::add_pointer_t<void(float, float)>;
	using BGUTRenderCallback = std::add_pointer_t<void()>;
	using BGUTEventHandlerCallback = std::add_pointer_t<void(const SDL_Event &)>;
	using BGUTWindowPtr = SDL_Window *;
	using BGUTWindowID = std::size_t;

	bool BGUTInit(std::string_view configFilename);
	bool BGUTCreateWindow(std::string_view windowTitle, std::string_view iconFilename);
	void BGUTSetWindow(BGUTWindowPtr pWindow);
	void BGUTMainLoop(void);
	void BGUTSendExitCode(int exitCode); // Signal termination
	void BGUTShutdown(void);
	void BGUTSetWindowFullscreen(BGUTWindowPtr pWindow, bool useFullscreen);
	void BGUTSetWindowSize(BGUTWindowPtr pWindow, int width, int height);
	// TODO: This responsibility should be handled by the renderer and the app layer.
	void BGUTSetViewport(int x, int y, int width, int height);
	void BGUTSetCallbackUpdate(BGUTUpdateCallback pUpdateCallback);
	void BGUTSetCallbackRender(BGUTRenderCallback pRenderCallback);
	void BGUTSetCallbackEventHandler(BGUTEventHandlerCallback pEventHandlerCallback);
	SDL_Window *BGUTGetWindowPtr(void); // BGUTWindowID windowID
	SDL_GLContext BGUTGetContextPtr(void);
	const Timer &BGUTGetMainLoopTimer(void);
	int BGUTGetExitCode(void); // App exit code
} // End namespace (BGE)

#define BGE_EXIT_SUCCESS 0 // Pass to BGUTSendExitCode()
#define BGE_EXIT_FAILURE 1

#endif /* !_BGE_BGUT_HPP_ */