/*=============================================================================*
 * Shader.hpp - Graphics shader facilities.
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
#ifndef _BGE_SHADER_HPP_
#define _BGE_SHADER_HPP_

namespace BGE
{
	class IShader;
	using ShaderMap = std::map<std::string, IShader *>;

	std::optional<GLuint> GetShaderUniformLocation(GLuint programID, std::string_view uniformName) noexcept;
		
	class IShader
	{
	public:
		virtual GLuint VCreate(void) = 0;
		virtual bool VCompile(void) = 0;
		virtual GLuint VGetID(void) = 0;
		virtual void VDestroy(void) = 0;
		virtual bool VIsValid(void) = 0;
	};
	
	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram(void) = default;
		virtual GLuint VCreate(void) = 0;
		virtual GLuint VGetID(void) const = 0;
		virtual void VAttachShader(const IShader &shader) = 0;
		virtual void VDetachShader(const IShader &shader) = 0;
		virtual bool VLink(void) = 0;
		virtual void VBind(void) = 0;
		virtual void VDestroy(void) = 0;
		virtual bool VIsValid(void) const = 0;
	};

	class ShaderProgram final : public IShaderProgram
	{
		GLuint m_programID;
		// TODO: A container of shaders should be used to keep track of linked shaders
	public:
		ShaderProgram(void);
		ShaderProgram(const ShaderProgram &) = delete; // Disable copy/move constructors & ops
		ShaderProgram &operator=(const ShaderProgram &) = delete;
		ShaderProgram(ShaderProgram &&) noexcept = delete;
		ShaderProgram &operator=(ShaderProgram &&) noexcept = delete;
		~ShaderProgram(void);

		virtual GLuint VCreate(void) override;
		virtual GLuint VGetID(void) const override;
		virtual void VAttachShader(const IShader &shader) override;
		virtual void VDetachShader(const IShader &shader) override;
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
			auto result = GetShaderUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
			if (result) glProgramUniform1f(m_programID, *result, vec2.AsArray().data());
		}
		template <Math::Numeric Type>
		void SetVec3(std::string_view uniformName, const Math::Vec3<Type> &vec3)
		{
			auto result = GetShaderUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
			if (result) glProgramUniform3f(m_programID, *result, vec3.AsArray().data());
		}
		template <Math::Numeric Type>
		void SetVec4(std::string_view uniformName, const Math::Vec4<Type> &vec4)
		{
			//auto result = GetShaderUniformLocation(m_programID, uniformName);
			//BGE_ASSERT(!result.has_value());
			//if (result) glProgramUniform4f(m_programID, *result, vec4);
		}

		template <Math::Numeric Type>
		void SetMat2x2(std::string_view uniformName, const Math::Mat2x2<Type> &mat2x2)
		{
			auto result = GetShaderUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
		template <Math::Numeric Type>
		void SetMat3x3(std::string_view uniformName, const Math::Mat3x3<Type> &mat3x3)
		{
			auto result = GetShaderUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
		template <Math::Numeric Type>
		void SetMat4x4(std::string_view uniformName, const Math::Mat4x4<Type> &mat4x4)
		{
			auto result = GetShaderUniformLocation(m_programID, uniformName);
			BGE_ASSERT(!result.has_value());
		}
	};
	
	class VertexShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit VertexShader(std::string_view shaderFilename);
		~VertexShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_VERTEX_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};

	class TessControlShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit TessControlShader(std::string_view shaderFilename);
		~TessControlShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_TESS_CONTROL_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};

	class TessEvalShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit TessEvalShader(std::string_view shaderFilename);
		~TessEvalShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_TESS_EVALUATION_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};

	class GeometryShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit GeometryShader(std::string_view shaderFilename);
		~GeometryShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_GEOMETRY_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};

	class FragmentShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit FragmentShader(std::string_view shaderFilename);
		~FragmentShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_FRAGMENT_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};

	class ComputeShader final : public IShader
	{
		GLuint m_programID;
		std::string m_shaderFilename;
	public:
		explicit ComputeShader(std::string_view shaderFilename);
		~ComputeShader(void) { VDestroy(); }

		virtual GLuint VCreate(void) override { return glCreateShader(GL_COMPUTE_SHADER); }
		virtual bool VCompile(void) override { return false; }
		virtual GLuint VGetID(void) override { return m_programID; }
		virtual void VDestroy(void) override { glDeleteShader(m_programID); }
		virtual bool VIsValid(void) override { return glIsShader(m_programID); }
	};
	// Template specializations for Math:: types:
	template <> // Int
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2i &vec2)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2i(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Float
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2f &vec2)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2f(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Double
	void ShaderProgram::SetVec2(std::string_view uniformName, const Math::Vec2d &vec2)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform2d(m_programID, *result, vec2.x, vec2.y);
	}

	template <> // Int
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3i &vec3)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3i(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Float
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3f &vec3)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3f(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Double
	void ShaderProgram::SetVec3(std::string_view uniformName, const Math::Vec3d &vec3)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform3d(m_programID, *result, vec3.x, vec3.y, vec3.z);
	}

	template <> // Int
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4i &vec4)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4i(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	template <> // Float
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4f &vec4)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4f(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	template <> // Double
	void ShaderProgram::SetVec4(std::string_view uniformName, const Math::Vec4d &vec4)
	{
		auto result = GetShaderUniformLocation(m_programID, uniformName);
		BGE_ASSERT(!result.has_value());
		if (result) glProgramUniform4d(m_programID, *result, vec4.x, vec4.y, vec4.z, vec4.w);
	}
} // End namespace (BGE)

#endif /* !_BGE_SHADER_HPP_ */
