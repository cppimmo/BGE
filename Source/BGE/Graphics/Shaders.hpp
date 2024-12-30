/*=============================================================================*
 * Shader.hpp - Graphics shader facilities.
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
#ifndef _BGE_SHADERS_HPP_
#define _BGE_SHADERS_HPP_

#include "Resources/ResourceHandle.hpp"

namespace BGE
{
	class IShader; // Forward declare
	BGE_DECLARE_PTR(IShader);
	class Shader; // Forward declare
	BGE_DECLARE_PTR(Shader);
	class VertexShader; // Forward declare
	BGE_DECLARE_PTR(VertexShader);
	class FragmentShader; // Forward declare
	BGE_DECLARE_PTR(FragmentShader);

	//! List of shaders.
	using ShaderList = std::list<StrongIShaderPtr>;
	//! Map of shaders to given names.
	using ShaderMap = std::map<std::string, StrongIShaderPtr>;

	/**
	 * @brief .
	 */
	class IShader
	{
	public:
		virtual ~IShader(void) = default;
		//! .
		virtual bool VCreate(void) = 0;
		//! .
		virtual bool VCompile(std::string_view source) = 0;
		//! .
		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) = 0;
		//! .
		virtual GLuint VGetID(void) const = 0;
		//! .
		virtual void VDestroy(void) = 0;
		//! .
		virtual bool VIsValid(void) const = 0;
	protected:
		static bool Compile(GLuint shaderID, std::string_view source);
	};
	
	// TODO: Shaders should use resource cache handles for shader source code.

	/**
	 * @brief .
	 */
	class Shader : public IShader
	{
	protected:
		GLuint m_shaderID;
	public:
		Shader(void);
		virtual ~Shader(void);
		// IShader's interface:
		virtual bool VCompile(std::string_view source) override;
		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) override;
		virtual GLuint VGetID(void) const override;
		virtual void VDestroy(void) override;
		virtual bool VIsValid(void) const override;
	};

	class VertexShader final : public Shader
	{
	public:
		VertexShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	class TessControlShader final : public Shader
	{
	public:
		TessControlShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	class TessEvalShader final : public Shader
	{
	public:
		TessEvalShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	class GeometryShader final : public Shader
	{
	public:
		GeometryShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	class FragmentShader final : public Shader
	{
	public:
		FragmentShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	class ComputeShader final : public Shader
	{
	public:
		ComputeShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_SHADERS_HPP_ */
