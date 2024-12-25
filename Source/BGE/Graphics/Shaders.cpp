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

BGE::Shader::~Shader(void)
{
    VDestroy();
}

GLuint BGE::Shader::VGetID(void) const
{
    return m_shaderID;
}

void BGE::Shader::VDestroy(void)
{
    glDeleteShader(m_shaderID);
}

bool BGE::Shader::VIsValid(void) const
{
    return glIsShader(m_shaderID);
}

bool BGE::VertexShader::VCreate(void)
{
    m_shaderID = glCreateShader(GL_VERTEX_SHADER);
    return m_shaderID != 0;
}

bool BGE::VertexShader::VCompile(std::string_view source)
{
    const char *pSourceData = source.data();
    const GLint kSourceLength = static_cast<GLint>(source.size());

    // Attach the source to the shader
    glShaderSource(m_shaderID, 1, &pSourceData, &kSourceLength);
    glCompileShader(m_shaderID); // Compile the shader

    // Check the shader compilation status
    GLint status{};
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        // Get the length of the shader info log
        GLint infoLogLength{};
        glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        // Retrieve the shader info log
        std::string infoLog(infoLogLength, '\0');
        GLsizei length{};
        glGetShaderInfoLog(m_shaderID, infoLogLength, &length, infoLog.data());

        // Log the error message
        BGE_LOG("Graphics", "Shader compilation failed: %s", infoLog.c_str());
        return false;
    }
    return true;
}
