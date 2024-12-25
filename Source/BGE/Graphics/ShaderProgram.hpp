#ifndef _BGE_SHADERPROGRAM_HPP_
#define _BGE_SHADERPROGRAM_HPP_

#include "Graphics/Shaders.hpp"

namespace BGE
{
	class IShaderProgram; // Forward declare
	BGE_DECLARE_PTR(IShaderProgram);

	/**
	 * @brief .
	 */
	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram(void) = default;
		// Interface:
		virtual bool VCreate(void) = 0;
		virtual GLuint VGetID(void) const = 0;
		virtual void VAttachShader(StrongIShaderPtr pShader) = 0;
		virtual void VDetachShader(StrongIShaderPtr pShader) = 0;
		virtual bool VLink(void) = 0;
		virtual void VBind(void) = 0;
		virtual void VDestroy(void) = 0;
		virtual bool VIsValid(void) const = 0;
	public:
		/**
		 *
		 */
		static std::optional<GLint> GetUniformLocation(GLuint programID, std::string_view uniformName) noexcept;
	};

	/**
	 * @brief .
	 */
	class ShaderProgram final : public IShaderProgram
	{
		GLuint m_programID;
		ShaderList m_shaders;
		// TODO: A container of shaders should be used to keep track of linked shaders
	public:
		ShaderProgram(void);
		ShaderProgram(const ShaderProgram &) = delete; // Disable copy/move constructors & ops
		ShaderProgram &operator=(const ShaderProgram &) = delete;
		ShaderProgram(ShaderProgram &&) noexcept = delete;
		ShaderProgram &operator=(ShaderProgram &&) noexcept = delete;
		~ShaderProgram(void);
		// IShaderProgram's interface:
		virtual bool VCreate(void) override;
		virtual GLuint VGetID(void) const override;
		virtual void VAttachShader(StrongIShaderPtr pShader) override;
		virtual void VDetachShader(StrongIShaderPtr pShader) override;
		virtual bool VLink(void) override;
		virtual void VBind(void) override;
		virtual void VDestroy(void) override;
		virtual bool VIsValid(void) const override;

		void SetBool(std::string_view uniformName, bool value);
		void SetInt(std::string_view uniformName, GLint value);
		void SetUnsignedInt(std::string_view uniformName, GLuint value);
		void SetFloat(std::string_view uniformName, GLfloat value);
		void SetDouble(std::string_view uniformName, GLdouble value);

		template <Math::Numeric Type>
		void SetVec2(std::string_view uniformName, const Math::Vec2<Type> &vec2)
		{
			auto result = GetUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
			if (result) glProgramUniform1f(m_programID, *result, vec2.AsArray().data());
		}
		template <Math::Numeric Type>
		void SetVec3(std::string_view uniformName, const Math::Vec3<Type> &vec3)
		{
			auto result = GetUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
			if (result) glProgramUniform3f(m_programID, *result, vec3.AsArray().data());
		}
		template <Math::Numeric Type>
		void SetVec4(std::string_view uniformName, const Math::Vec4<Type> &vec4)
		{
			//auto result = GetUniformLocation(m_programID, uniformName);
			//BGE_ASSERT(!result.has_value());
			//if (result) glProgramUniform4f(m_programID, *result, vec4);
		}

		template <Math::Numeric Type>
		void SetMat2x2(std::string_view uniformName, const Math::Mat2x2<Type> &mat2x2)
		{
			auto result = GetUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
		template <Math::Numeric Type>
		void SetMat3x3(std::string_view uniformName, const Math::Mat3x3<Type> &mat3x3)
		{
			auto result = GetUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
		template <Math::Numeric Type>
		void SetMat4x4(std::string_view uniformName, const Math::Mat4x4<Type> &mat4x4)
		{
			auto result = GetUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
	};

	// Template specializations for Math:: types:
	template <> // Int
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2i &vec2)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2i(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Float
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2f &vec2)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2f(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Double
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2d &vec2)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2d(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Int
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3i &vec3)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3i(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Float
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3f &vec3)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3f(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Double
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3d &vec3)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3d(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Int
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4i &vec4)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4i(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	template <> // Float
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4f &vec4)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4f(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	template <> // Double
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4d &vec4)
	{
		auto result = GetUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4d(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}
} // End namespace (BGE)

#endif /* !_BGE_SHADERPROGRAM_HPP_ */
