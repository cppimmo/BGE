#ifndef _BGE_SHADERPROGRAM_HPP_
#define _BGE_SHADERPROGRAM_HPP_

#include "Graphics/Bindable.hpp"
#include "Graphics/Shaders.hpp"

namespace BGE
{
	class IShaderProgram; // Forward declare
	BGE_DECLARE_PTR(IShaderProgram);
	class ShaderProgram; // Forward declare
	BGE_DECLARE_PTR(ShaderProgram);

	/**
	 * @brief .
	 */
	class IShaderProgram : public IBindable
	{
	public:
		virtual ~IShaderProgram(void) = default;
		// Interface:
		virtual bool VCreate(void) = 0;
		virtual GLuint VGetID(void) const = 0;
		virtual void VAttachShader(StrongIShaderPtr pShader) = 0;
		virtual void VDetachShader(StrongIShaderPtr pShader) = 0;
		virtual bool VLink(void) = 0;
		virtual void VDestroy(void) = 0;
		virtual bool VIsValid(void) const = 0;
		virtual void VSetBool(std::string_view uniformName, bool value) = 0;
		virtual void VSetInt(std::string_view uniformName, int value) = 0;
		virtual void VSetUnsignedInt(std::string_view uniformName, unsigned int value) = 0;
		virtual void VSetFloat(std::string_view uniformName, float value) = 0;
		virtual void VSetDouble(std::string_view uniformName, double value) = 0;
		virtual void VSetVec2(std::string_view uniformName, const glm::vec2   &kValue) = 0;
		virtual void VSetVec3(std::string_view uniformName, const glm::vec3   &kValue) = 0;
		virtual void VSetVec4(std::string_view uniformName, const glm::vec4   &kValue) = 0;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2   &kValue) = 0;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2x3 &kValue) = 0;
		virtual void VSetMat2(std::string_view uniformName, const glm::mat2x4 &kValue) = 0;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3   &kValue) = 0;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3x2 &kValue) = 0;
		virtual void VSetMat3(std::string_view uniformName, const glm::mat3x4 &kValue) = 0;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4   &kValue) = 0;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4x2 &kValue) = 0;
		virtual void VSetMat4(std::string_view uniformName, const glm::mat4x3 &kValue) = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_SHADERPROGRAM_HPP_ */
