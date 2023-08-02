/*=============================================================================*
 * Shader.cpp - Graphics shader facilities.
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
#include "Shaders.hpp"

BGE::ShaderProgram::ShaderProgram(void)
    : m_programID(0)
{
}

BGE::ShaderProgram::~ShaderProgram(void)
{
    if (VIsValid()) VDestroy(); // Ensure the program is destroyed
}

GLuint BGE::ShaderProgram::VCreate(void)
{
    return GLuint();
}

GLuint BGE::ShaderProgram::VGetID(void) const
{
    return m_programID;
}

void BGE::ShaderProgram::VAttachShader(const IShader &shader)
{
}

void BGE::ShaderProgram::VDetachShader(const IShader &shader)
{
}

bool BGE::ShaderProgram::VLink(void)
{
    // TODO: Link all attached shader objects to this program.
    return false;
}

void BGE::ShaderProgram::VBind(void)
{
    glUseProgram(m_programID);
}

void BGE::ShaderProgram::VDestroy(void)
{
    glDeleteProgram(m_programID);
}

bool BGE::ShaderProgram::VIsValid(void) const
{
    return glIsProgram(m_programID);
}

void BGE::ShaderProgram::SetBool(std::string_view uniformName, bool value)
{
    auto result = GetShaderUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result) glProgramUniform1i(m_programID, *result, static_cast<bool>(value));
}

void BGE::ShaderProgram::SetInt(std::string_view uniformName, GLint value)
{
    auto result = GetShaderUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result) glProgramUniform1i(m_programID, *result, value);
}

void BGE::ShaderProgram::SetUnsignedInt(std::string_view uniformName, GLuint value)
{
    auto result = GetShaderUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result) glProgramUniform1ui(m_programID, *result, value);
}

void BGE::ShaderProgram::SetFloat(std::string_view uniformName, GLfloat value)
{
    auto result = GetShaderUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result) glProgramUniform1f(m_programID, *result, value);
}

void BGE::ShaderProgram::SetDouble(std::string_view uniformName, GLdouble value)
{
    auto result = GetShaderUniformLocation(m_programID, uniformName);
    BGE_ASSERT(!result.has_value());
    if (result) glProgramUniform1d(m_programID, *result, value);
}

std::optional<GLuint> BGE::GetShaderUniformLocation(GLuint programID, std::string_view uniformName) noexcept
{
    auto result = glGetUniformLocation(programID, uniformName.data());
    return (result < 0) ? std::nullopt : std::optional<GLuint>(result);
}