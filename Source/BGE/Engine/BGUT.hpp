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
#ifndef _BGE_BGUT_HPP_
#define _BGE_BGUT_HPP_

#include <cstdint>

#include "Engine/EngineStd.hpp"

namespace BGE
{
	struct EngineOptions; // Forward declare

	// 1st Arg (delta time milliseconds), 2nd Arg (elapsed time milliseconds)
	using BGUTUpdateCallback = std::add_pointer_t<void(float, float)>;
	using BGUTRenderCallback = std::add_pointer_t<void(float, float)>;
	using BGUTEventHandlerCallback = std::add_pointer_t<bool(const SDL_Event &)>;
	using BGUTResizeCallback = std::add_pointer_t<void(std::int32_t, std::int32_t)>;
	using BGUTWindowPtr = SDL_Window *;
	using BGUTWindowID = std::size_t;

	inline constexpr int kBGE_EXIT_SUCCESS = 0; // Pass to BGUTSendExitCode()
	inline constexpr int kBGE_EXIT_FAILURE = 1;

	bool BGUTInit(const EngineOptions &kOptions);
	bool BGUTCreateWindow(std::string_view windowTitle, std::string_view iconFilename);
	void BGUTSetWindow(BGUTWindowPtr pWindow);
	void BGUTMainLoop(void);
	void BGUTSendExitCode(int exitCode); // Signal termination
	void BGUTShutdown(void);
	void BGUTSetWindowTitle(std::string_view title);
	void BGUTSetWindowFullscreen(BGUTWindowPtr pWindow, bool bUseFullscreen);
	void BGUTSetWindowIcon(const std::filesystem::path &kFilePath);
	void BGUTSetWindowSize(BGUTWindowPtr pWindow, int width, int height);
	// TODO: This responsibility should be handled by the renderer and the app layer.
	void BGUTSetCallbackUpdate(BGUTUpdateCallback pUpdateCallback);
	void BGUTSetCallbackRender(BGUTRenderCallback pRenderCallback);
	void BGUTSetCallbackEventHandler(BGUTEventHandlerCallback pEventHandlerCallback);
	void BGUTSetCallbackResize(BGUTResizeCallback pResizeCallback);
	SDL_Window *BGUTGetWindowPtr(void); // BGUTWindowID windowID
	void BGUTGetWindowSize(BGUTWindowPtr pWindow, int &width, int &height);
	SDL_GLContext BGUTGetContextPtr(void);
	void BGUTSetImGuiContextPtrs(ImGuiContext *pImGuiContext, ImPlotContext *pImPlotContext);
	ImGuiContext *BGUTGetImGuiContextPtr(void);
	ImPlotContext *BGUTGetImPlotContextPtr(void);
	const Timer &BGUTGetMainLoopTimer(void);
	int BGUTGetExitCode(void); // App exit code
} // End namespace (BGE)

#endif /* !_BGE_BGUT_HPP_ */
