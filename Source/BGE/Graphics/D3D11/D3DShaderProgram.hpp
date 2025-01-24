#ifndef _BGE_GRAPHICS_D3D11_D3DSHADERPROGRAM_HPP_
#define _BGE_GRAPHICS_D3D11_D3DSHADERPROGRAM_HPP_

#include "Graphics/ShaderProgram.hpp"

namespace BGE
{
	class D3DShaderProgram final : public IShaderProgram
	{
	private:

	public:
		D3DShaderProgram(void);
		~D3DShaderProgram(void) override;
		// IShaderProgram's interface:
		void VBind(void) override;
		void VUnbind(void) override;
		bool VCreate(void) override;
		GLuint VGetID(void) const override;
		void VAttachShader(StrongIShaderPtr pShader) override;
		void VDetachShader(StrongIShaderPtr pShader) override;
		bool VLink(void) override;
		void VDestroy(void) override;
		bool VIsValid(void) const override;
		void VSetBool(std::string_view uniformName, bool value) override;
		void VSetInt(std::string_view uniformName, int value) override;
		void VSetUnsignedInt(std::string_view uniformName, unsigned int value) override;
		void VSetFloat(std::string_view uniformName, float value) override;
		void VSetDouble(std::string_view uniformName, double value) override;
		void VSetVec2(std::string_view uniformName, const glm::vec2 &kValue) override;
		void VSetVec3(std::string_view uniformName, const glm::vec3 &kValue) override;
		void VSetVec4(std::string_view uniformName, const glm::vec4 &kValue) override;
		void VSetMat2(std::string_view uniformName, const glm::mat2 &kValue) override;
		void VSetMat3(std::string_view uniformName, const glm::mat3 &kValue) override;
		void VSetMat4(std::string_view uniformName, const glm::mat4 &kValue) override;
		void VSetStruct(std::string_view uniformName, const UniformStruct &kStruct, const void *pStructData) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_GRAPHICS_D3D11_D3DSHADERPROGRAM_HPP_ */