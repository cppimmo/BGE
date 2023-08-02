/*=============================================================================*
 * Debug.hpp - Graphics debugging facilities.
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
#ifndef _BGE_GRAPHICS_DEBUG_HPP_
#define _BGE_GRAPHICS_DEBUG_HPP_

#include "Engine/EngineStd.hpp"

namespace BGE::GL
{
/**
 * Reset the OpenGL internal error state variable.
 */
void ClearErrors(void);
/**
 * Use glDebugMessageCallback(DEBUGPROC callback, void *pUserParam)
 * Use glEnable/glDisable(GL_DEBUG_OUTPUT)
 * Use glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS) to enable output in realtime and disable caching.
 */
void APIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
								   const GLchar *pMessage, const void *pUserParam);
/**
 * This function doesn't really do the heavy lifting that's needed.  The context first needs to be set
 * by the windowing system (SDL abstracts it in this case), otherwise the functionality could be limited.
 */
void DebugContextSetup(void);

} // End namespace (BGE::GL)

#endif /* !_BGE_GRAPHICS_DEBUG_HPP_ */