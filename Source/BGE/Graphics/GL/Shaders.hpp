#ifndef _BGE_GL_SHADERS_HPP_
#define _BGE_GL_SHADERS_HPP_

#include "Graphics/Shaders.hpp"

namespace BGE
{
	class GLShader; // Forward declare
	BGE_DECLARE_PTR(GLShader);
	class GLShaderFactory; // Forward declare
	BGE_DECLARE_PTR(GLShaderFactory);

	class GLVertexShader; // Forward declare
	BGE_DECLARE_PTR(GLVertexShader);
	class GLTessControlShader; // Forward declare
	BGE_DECLARE_PTR(GLTessControlShader);
	class GLTessEvalShader; // Forward declare
	BGE_DECLARE_PTR(GLTessEvalShader);
	class GLGeometryShader; // Forward declare
	BGE_DECLARE_PTR(GLGeometryShader);
	class GLFragmentShader; // Forward declare
	BGE_DECLARE_PTR(GLFragmentShader);
	class GLComputeShader; // Forward declare
	BGE_DECLARE_PTR(GLComputeShader);

	/**
	 * @brief .
	 */
	class GLShader : public IShader
	{
	protected:
		GLuint m_shaderID;
	public:
		GLShader(void);
		virtual ~GLShader(void) override;
		// IShader's interface:
		virtual bool VCompile(std::string_view source) override;
		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) override;
		virtual bool VCompileBinary(StrongResourceHandlePtr pResourceHandle, std::string_view entryPoint) override;
		virtual GLuint VGetID(void) const override;
		virtual void VDestroy(void) override;
		virtual bool VIsValid(void) const override;
	protected:
		static bool Compile(GLuint shaderID, std::string_view source);
	};

	/**
	 * @brief .
	 */
	class GLShaderFactory final : public IShaderFactory
	{
	public:
		virtual ~GLShaderFactory(void) override = default;
		// Interface:
		virtual StrongIShaderPtr VCreateVertexShader(void) const override;
		virtual StrongIShaderPtr VCreateTessControlShader(void) const override;
		virtual StrongIShaderPtr VCreateTessEvalShader(void) const override;
		virtual StrongIShaderPtr VCreateGeometryShader(void) const override;
		virtual StrongIShaderPtr VCreateFragmentShader(void) const override;
		virtual StrongIShaderPtr VCreateComputeShader(void) const override;
	};

	/**
	 * @brief .
	 */
	class GLVertexShader final : public GLShader
	{
	public:
		GLVertexShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	/**
	 * @brief .
	 */
	class GLTessControlShader final : public GLShader
	{
	public:
		GLTessControlShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	/**
	 * @brief .
	 */
	class GLTessEvalShader final : public GLShader
	{
	public:
		GLTessEvalShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	/**
	 * @brief .
	 */
	class GLGeometryShader final : public GLShader
	{
	public:
		GLGeometryShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	/**
	 * @brief .
	 */
	class GLFragmentShader final : public GLShader
	{
	public:
		GLFragmentShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};

	/**
	 * @brief .
	 */
	class GLComputeShader final : public GLShader
	{
	public:
		GLComputeShader(void) = default;
		// IShader's interface:
		virtual bool VCreate(void) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_SHADERS_HPP_ */
