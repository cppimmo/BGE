#ifndef _BGE_GRAPHICS_GL_SHADERBUFFER_HPP_
#define _BGE_GRAPHICS_GL_SHADERBUFFER_HPP_

#include "Graphics/ShaderBuffer.hpp"

namespace BGE
{
	template <ShaderBufferDataType Type>
	class GLShaderBuffer; // Forward declare
	BGE_DECLARE_MULTITEMPLATED_PTR(GLShaderBuffer);

	/**
	 * @brief 
	 * @tparam Type 
	 */
	template <ShaderBufferDataType Type>
	class GLShaderBuffer final : public IShaderBuffer<Type>
	{
		using IShaderBuffer<Type>::m_shaderType;
		using IShaderBuffer<Type>::m_bufferData;
	private:
		GLuint m_bufferID = 0u;
	public:
		explicit GLShaderBuffer(ShaderType shaderType) : IShaderBuffer<Type>(shaderType) { }
		GLShaderBuffer(ShaderType shaderType, const Type &kData) : IShaderBuffer<Type>(shaderType, kData) { }
		~GLShaderBuffer(void) override { VDestroy(); }
		// IShaderBuffer's interface:
		virtual bool VCreate(void) override
		{
			glCreateBuffers(1, &m_bufferID);
			glNamedBufferData(m_bufferID, sizeof(Type), &m_bufferData, GL_DYNAMIC_DRAW);
		}
		
		virtual void Update(const Type &kData) override
		{
			m_bufferData = kData;
			glNamedBufferSubData(m_bufferID, 0, sizeof(Type), &m_bufferData);
		}

		virtual void VBind(std::uint32_t slot) const override
		{
			switch (m_shaderType)
			{
			case ShaderType::kVertex:
			case ShaderType::kHull:
			case ShaderType::kDomain:
			case ShaderType::kGeometry:
			case ShaderType::kPixel:
			case ShaderType::kCompute:
				break;
			default:
				BGE_ASSERT(false && "Invalid shader type");
				break;
			}

			glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_bufferID);
		}

		virtual void VDestroy(void) override
		{
			glDeleteBuffers(1, &m_bufferID);
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_GL_SHADERBUFFER_HPP_ */