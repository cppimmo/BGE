/*=============================================================================*
 * ShaderBuffer.hpp - .
 *
 * Copyright (c) 2025, Brian Hoffpauir All rights reserved.
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
#ifndef _BGE_GRAPHICS_SHADERBUFFER_HPP_
#define _BGE_GRAPHICS_SHADERBUFFER_HPP_

namespace BGE
{
	struct IShaderBufferData
	{
		virtual ~IShaderBufferData(void) = default;
	};
	template <typename Type>
	concept DerivedFromIShaderBufferData = std::derived_from<Type, IShaderBufferData>;

	struct ShaderBufferData_WorldViewProjection : public IShaderBufferData
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
		std::int32_t _padding[2];
	};

	/**
	 * @brief Wrapper around a shader buffer object. Can be applied to DirectX 11's constant
	 * buffers or OpenGL's uniform buffers.
	 * @tparam Type 
	 */
	template <DerivedFromIShaderBufferData Type>
	class IShaderBuffer : public INonCopyable, public INonMovable
	{
	protected:
		Type m_bufferData; //!< Stored data specific to the buffer type.
	public:
		IShaderBuffer(void) = default;
		IShaderBuffer(const Type &kData) : m_bufferData(kData) { }
		virtual ~IShaderBuffer(void) = default;
		// Interface:
		virtual bool VCreate(void) = 0;
		virtual void Update(const Type &kData) = 0;
		virtual void VBind(std::uint32_t slot) const = 0;
		virtual void VDestroy(void) = 0;
		// Accessors:
		const Type &GetData(void) const { return m_bufferData; }
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_SHADERBUFFER_HPP_ */
