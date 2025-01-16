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
	struct EngineOptions; // Forward declare
	class IRenderer; // Forward declare;
	BGE_DECLARE_PTR(IRenderer);

	//! Enum for representing a renderer implementation.
	enum struct RendererImpl
	{
		kOpenGL, /**< OpenGL renderer. */
		kD3D11   /**< DirectX 11 renderer. */
	};

	enum struct BlendMode
	{
		kAdditive,
		kSubtractive
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer(void) = default;

		virtual bool VInit(const EngineOptions &kOptions) = 0;
		virtual void VShutdown(void) = 0;
		virtual RendererImpl VGetImpl(void) const = 0;

		virtual void VBeginFrame(void) = 0; // Begin a new rendering frame.
		virtual void VEndFrame(void) = 0; // End the frame and present the back buffer.

		virtual void VOnResize(std::int32_t width, std::int32_t height) = 0;
		virtual void VSetViewport(const IViewport &kViewport) = 0;
		virtual const IViewport &VGetViewport(void) const = 0;
		virtual void VSetBackgroundColor(const glm::vec4 &kColor) = 0;
		virtual glm::vec4 VGetBackgroundColor(void) = 0;

		//virtual StrongITexturePtr VLoadTexture(const std::string& filepath) = 0;
		//virtual void VBindTexture(StrongITexturePtr texture, unsigned int slot) = 0;

		//virtual StrongIVertexBufferPtr VCreateVertexBuffer(const std::vector<Vertex>& vertices) = 0;
		//virtual StrongIIndexBufferPtr VCreateIndexBuffer(const std::vector<uint32_t>& indices) = 0;
		//virtual void VDrawIndexed(unsigned int indexCount) = 0;

		virtual void VEnableDepthTest(bool bEnable) = 0; // Enable/disable depth testing.
		virtual void VEnableBlending(bool bEnable) = 0; // Enable/disable blending.
		virtual void VSetBlendMode(BlendMode mode) = 0; // Set blending mode (e.g., additive, subtractive).
		virtual BlendMode VGetBlendMode(void) const = 0;

		virtual StrongIShaderFactoryPtr VCreateShaderFactory(void) = 0;
		virtual StrongIShaderProgramPtr VCreateShaderProgram(std::string_view name) = 0;
		virtual StrongIShaderProgramPtr VGetShaderProgram(std::string_view name) = 0;

		virtual bool VTakeScreenshot(const std::filesystem::path &kSaveGameDir) = 0;

		virtual void VEnableDebugOutput(bool bEnable) = 0; // Enable OpenGL debug context messages.
		virtual std::string VGetRendererInfo(void) const = 0; // Return GPU/driver information.
	protected:
		static std::string GetScreenshotFilename(std::string_view saveGameDir);
	};
} // End namespace (BGE)

#endif /* !_BGE_RENDERER_HPP_ */
