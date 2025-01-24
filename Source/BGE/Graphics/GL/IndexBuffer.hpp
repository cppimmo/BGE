#ifndef _BGE_GRAPHICS_GL_INDEXBUFFER_HPP_
#define _BGE_GRAPHICS_GL_INDEXBUFFER_HPP_

#include "Graphics/IndexBuffer.hpp"
#include "Graphics/GL/Renderer.hpp"

namespace BGE
{
	template <typename IndexType>
	requires std::integral<IndexType> // Ensure only integral types (e.g., std::uint16_t, std::uint32_t)
	class GLIndexBuffer final : public IIndexBuffer<IndexType>
	{
	private:
		GLuint m_bufferID = 0;
		std::size_t m_indexCount = 0;
	public:
		GLIndexBuffer(void) = default;

		~GLIndexBuffer(void) override
		{
			VDestroy();
		}

		virtual bool VCreate(const std::vector<IndexType> &kIndices, bool bDynamic) override
		{
			glCreateBuffers(1, &m_bufferID);
			GLbitfield flags = (bDynamic ? GL_DYNAMIC_STORAGE_BIT : 0u);
			glNamedBufferStorage(m_bufferID, kIndices.size() * sizeof(IndexType), kIndices.data(), flags);
			m_indexCount = kIndices.size();
			return glIsBuffer(m_bufferID);
		}

		virtual void VBind(void) const override
		{
			// TODO: Use glVertexArrayElementBuffer(vao, ebo).
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
		}

		virtual void VUpdate(const std::vector<IndexType> &kIndices) override
		{
			// TODO: Check if dynamic storage bit was set.
			GLintptr offset = 0;
			glNamedBufferSubData(m_bufferID, offset, kIndices.size() * sizeof(IndexType), kIndices.data());
			m_indexCount = kIndices.size();
		}

		virtual void VDestroy(void) override
		{
			if (glIsBuffer(m_bufferID))
			{
				glDeleteBuffers(1, &m_bufferID);
				m_bufferID = 0;
			}
		}

		virtual std::size_t VGetIndexCount(void) const noexcept override
		{
			return m_indexCount;
		}

		virtual std::uintptr_t VGetNativeHandle(void) const override
		{
			return static_cast<std::uintptr_t>(m_bufferID);
		}
	};
}

#endif /* !_BGE_GRAPHICS_GL_INDEXBUFFER_HPP_ */