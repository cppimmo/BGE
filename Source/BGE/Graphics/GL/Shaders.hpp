#ifndef _BGE_GL_SHADERS_HPP_
#define _BGE_GL_SHADERS_HPP_

#include "Graphics/Shaders.hpp"

namespace BGE
{
	class GLShader; // Forward declare
	BGE_DECLARE_PTR(GLShader);
	class GLShaderFactory; // Forward declare
	BGE_DECLARE_PTR(GLShaderFactory);

	/**
	 * @brief .
	 */
	class GLShader : public IShader
	{
	protected:
		GLuint m_shaderID;
	public:
		explicit GLShader(ShaderType shaderType);
		virtual ~GLShader(void) override;
		// IShader's interface:
		virtual bool VCreate(void) override;
		virtual bool VCompile(StrongResourceHandlePtr pResourceHandle) override;
		virtual void *VGetBlob(void) const override;
		virtual void VDestroy(void) override;
		virtual bool VIsValid(void) const override;
		// Accessors:
		GLuint GetID(void) const noexcept { return m_shaderID; }
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
		virtual StrongIShaderPtr VCreateHullShader(void) const override;
		virtual StrongIShaderPtr VCreateDomainShader(void) const override;
		virtual StrongIShaderPtr VCreateGeometryShader(void) const override;
		virtual StrongIShaderPtr VCreatePixelShader(void) const override;
		virtual StrongIShaderPtr VCreateComputeShader(void) const override;
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_SHADERS_HPP_ */
