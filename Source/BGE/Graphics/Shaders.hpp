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
	class IShaderFactory; // Forward declare
	BGE_DECLARE_PTR(IShaderFactory);

	//! List of shaders.
	using ShaderList = std::list<StrongIShaderPtr>;
	//! Map of shaders to given names.
	using ShaderMap = std::map<std::string, StrongIShaderPtr>;

	//! .
	enum struct ShaderType
	{
		kVertex,
		kHull,
		kDomain,
		kGeometry,
		kPixel,
		kCompute
	};

	/**
	 * @brief .
	 */
	class IShader
	{
	protected:
		ShaderType m_type;
	public:
		explicit IShader(ShaderType shaderType) : m_type(shaderType) { }
		virtual ~IShader(void) = default;
		// Interface:
		//! .
		virtual bool VCreate(void) = 0;
		//! .
		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) = 0;
		//! .
		virtual void *VGetBlob(void) const = 0;
		//! .
		virtual void VDestroy(void) = 0;
		//! .
		virtual bool VIsValid(void) const = 0;

		ShaderType GetType(void) const noexcept { return m_type; }
	};

	class IShaderFactory
	{
	public:
		virtual ~IShaderFactory(void) = default;
		// Interface:
		virtual StrongIShaderPtr VCreateVertexShader(void) const = 0;
		virtual StrongIShaderPtr VCreateHullShader(void) const = 0; // Tess Control
		virtual StrongIShaderPtr VCreateDomainShader(void) const = 0; // Tess Eval
		virtual StrongIShaderPtr VCreateGeometryShader(void) const = 0;
		virtual StrongIShaderPtr VCreatePixelShader(void) const = 0; // Fragment
		virtual StrongIShaderPtr VCreateComputeShader(void) const = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_SHADERS_HPP_ */
