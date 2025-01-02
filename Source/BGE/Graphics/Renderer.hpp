/*=============================================================================*
 * Renderer.hpp - Graphics rendering.
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
#ifndef _BGE_RENDERER_HPP_
#define _BGE_RENDERER_HPP_

#include "Graphics/Viewport.hpp"
#include "Graphics/Shaders.hpp"
#include "Graphics/ShaderProgram.hpp"

namespace BGE
{
	class IRenderer; // Forward declare;
	BGE_DECLARE_PTR(IRenderer);

	//! Enum for representing a renderer implementation.
	enum struct RendererImpl
	{
		kOpenGL /**< OpenGL renderer. */
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer(void) = default;

		virtual bool VInit(void) = 0;
		virtual void VShutdown(void) = 0;
		virtual RendererImpl VGetImpl(void) const = 0;

		virtual StrongIShaderFactoryPtr VCreateShaderFactory(void) = 0;
		virtual StrongIShaderProgramPtr VCreateShaderProgram(std::string_view name) = 0;
		virtual StrongIShaderProgramPtr VGetShaderProgram(std::string_view name) = 0;

		virtual void VSetViewport(const IViewport &kViewport) = 0;
		virtual const IViewport &VGetViewport(void) const = 0;
		virtual void VSetBackgroundColor(const glm::vec4 &kColor) = 0;
		virtual glm::vec4 VGetBackgroundColor(void) = 0;

		virtual bool VTakeScreenshot(void) = 0;
		//virtual void VSetBackgroundColor(float r, float g, float b, float a) = 0;
		//virtual void VShutdown(void) = 0;
		//virtual bool VPreRender(void) = 0;
	protected:
	private:
	};

	int GetMaxVertexAttribs(void);
} // End namespace (BGE)

#endif /* !_BGE_RENDERER_HPP_ */
