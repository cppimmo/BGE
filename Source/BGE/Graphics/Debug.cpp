/*=============================================================================*
 * Debug.cpp - Graphics debugging facilities.
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
#include "Debug.hpp"

void BGE::GL::ClearErrors(void)
{
	GLenum error;
	// Pop all errors off the internal stack
	while ((error = glGetError()) != GL_NO_ERROR);
}

void BGE::GL::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
									const GLchar *pMessage, const void *pUserParam)
{
	using namespace std::literals::string_view_literals;
	// Assign strings for enums
	std::string_view sourceName;
	std::string_view typeName;
	std::string_view severityName;
	// Decide source string
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		sourceName = "API"sv;
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		sourceName = "WINDOW SYS"sv;
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		sourceName = "SHADER COMP"sv;
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		sourceName = "3RD PARTY"sv;
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		sourceName = "APP"sv;
		break;
	// Supply for SOURCE_OTHER and default case:
	case GL_DEBUG_SOURCE_OTHER:
	default:
		sourceName = "OTHER"sv;
		break;
	}
	// Decide type string
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		typeName = "ERROR"sv;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typeName = "DB"sv;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeName = "UB"sv;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typeName = "PERFORMANCE"sv;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typeName = "PORTABILITY"sv;
		break;
	case GL_DEBUG_TYPE_MARKER:
		typeName = "MARKER"sv;
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		typeName = "PUSH GROUP"sv;
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		typeName = "POP GROUP"sv;
		break;
	// Supply for TYPE_OTHER and default case:
	case GL_DEBUG_TYPE_OTHER:
	default:
		typeName = "OTHER"sv;
		break;
	}
	// Log format string
	static constexpr std::string_view c_kGL_DEBUG_FMT = "GL_DEBUG(ID:0x%08X)|%s|%s|%s: %s";
	// Weird use of preprocessor, please forgive me
#define BGE_GL_DEBUG_ARGS id, sourceName.data(), typeName.data(), severityName.data(), pMessage
	// Decide severity string & how message will be output
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		severityName = "HIGH";
		BGE_ERROR(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severityName = "MEDIUM";
		BGE_WARNING(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		severityName = "LOW";
		BGE_INFO(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		severityName = "NOTIF";
		BGE_INFO(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
		break;
	}
}

void BGE::GL::DebugContextSetup(void)
{
	glDebugMessageCallback(GL::DebugMessageCallback, nullptr);
	// Listen to everything
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	// However disable notification severity
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	// This isn't needed if an "official" context has already been hinted by the windowing system
	glEnable(GL_DEBUG_OUTPUT);
}
