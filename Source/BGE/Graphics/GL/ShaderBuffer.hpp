#ifndef _BGE_GRAPHICS_GL_SHADERBUFFER_HPP_
#define _BGE_GRAPHICS_GL_SHADERBUFFER_HPP_

#include "Graphics/ShaderBuffer.hpp"

namespace BGE
{
	
	template <DerivedFromIShaderBufferData Type>
	class GLShaderBuffer final : public IShaderBuffer<Type>
	{
	private:
		GLuint m_bufferID = 0u;
	public:
		GLShaderBuffer(void) = default;
		GLShaderBuffer(const Type &kData) : IShaderBuffer<Type>(kData) { }
		~GLShaderBuffer(void) override { VDestroy(); }
		// IShaderBuffer's interface:
		virtual bool VCreate(void) override
		{
			glCreateBuffers(1, &m_bufferID);
			glNamedBufferData(m_bufferID, sizeof(Type), &IShaderBuffer<Type>::m_bufferData, GL_DYNAMIC_DRAW);
		}
		virtual void Update(const Type &kData) override
		{
			IShaderBuffer<Type>::m_bufferData = kData;
			glNamedBufferSubData(m_bufferID, 0, sizeof(Type), &IShaderBuffer<Type>::m_bufferData);
		}
		virtual void VBind(std::uint32_t slot) const override
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_bufferID);
		}
		virtual void VDestroy(void) override
		{
			glDeleteBuffers(1, &m_bufferID);
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_GL_SHADERBUFFER_HPP_ */