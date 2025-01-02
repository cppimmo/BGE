#ifndef _BGE_GL_SHADERPROGRAM_HPP_
#define _BGE_GL_SHADERPROGRAM_HPP_

#include "Graphics/Shaders.hpp"
#include "Graphics/ShaderProgram.hpp"

namespace BGE
{
	class GLShaderProgram; // Forward declare
	BGE_DECLARE_PTR(GLShaderProgram);

	/**
	 * @brief .
	 */
	class GLShaderProgram final : public IShaderProgram, public INonCopyable, public INonMovable
	{
		using UniformLocationMap = std::unordered_map<std::string, GLint>;

		GLuint m_programID;
		UniformLocationMap m_uniformLocations;
		ShaderList m_shaders;
		// TODO: A container of shaders should be used to keep track of linked shaders
	public:
		GLShaderProgram(void);
		~GLShaderProgram(void) override;
		// IShaderProgram's interface:
		virtual bool VCreate(void) override;
		virtual GLuint VGetID(void) const override;
		virtual void VAttachShader(StrongIShaderPtr pShader) override;
		virtual void VDetachShader(StrongIShaderPtr pShader) override;
		virtual bool VLink(void) override;
		virtual void VBind(void) override;
		virtual void VUnbind(void) override;
		virtual void VDestroy(void) override;
		virtual bool VIsValid(void) const override;
		// Uniform accessors:
		virtual void VSetBool(std::string_view uniformName, bool value) override;
		virtual void VSetInt(std::string_view uniformName, int value) override;
		virtual void VSetUnsignedInt(std::string_view uniformName, unsigned int value) override;
		virtual void VSetFloat(std::string_view uniformName, float value) override;
		virtual void VSetDouble(std::string_view uniformName, double value) override;
		virtual void VSetVec2(std::string_view uniformName, const glm::vec2   &kValue) override;
		virtual void VSetVec3(std::string_view uniformName, const glm::vec3   &kValue) override;
		virtual void VSetVec4(std::string_view uniformName, const glm::vec4   &kValue) override;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2   &kValue) override;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2x3 &kValue) override;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2x4 &kValue) override;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3   &kValue) override;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3x2 &kValue) override;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3x4 &kValue) override;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4   &kValue) override;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4x2 &kValue) override;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4x3 &kValue) override;
	private:
		static std::optional<GLint> GetUniformLocation(GLuint programID, std::string_view uniformName) noexcept;

		void UpdateUniformLocation(std::string_view uniformName);
		int UniformLocation(std::string_view uniformName);
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_SHADERPROGRAM_HPP_ */
