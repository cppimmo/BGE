#include "Engine/EngineStd.hpp"
#include "Graphics/Material.hpp"

namespace BGE
{
	Material::Material(const Data &kData)
		: m_data(kData)
	{
	}

	glm::vec3 Material::GetAmbient(void) const
	{
		return glm::vec3(m_data.ambient);
	}

	glm::vec3 Material::GetDiffuse(void) const
	{
		return glm::vec3(m_data.diffuse);
	}

	void Material::GetSpecular(glm::vec3 &color, float &power) const
	{
        color = glm::vec3(m_data.specular);
        power = m_data.specular.a;
	}

	glm::vec3 Material::GetEmissive(void) const
	{
		return glm::vec3(m_data.emissive);
	}

	float Material::GetAlpha(void) const
	{
		return m_data.diffuse.a;
	}

	bool Material::HasAlpha(void) const
	{
		return GetAlpha() < 1.0f;
	}

	const Material::Data &Material::GetData(void) const
	{
		return m_data;
	}

	void Material::SetAmbient(const glm::vec3 &kColor)
	{
		m_data.ambient = glm::vec4(kColor, 1.0f);
	}

	void Material::SetDiffuse(const glm::vec3 &kColor)
	{
		m_data.diffuse = glm::vec4(kColor, m_data.diffuse.a);
	}

	void Material::SetSpecular(const glm::vec3 &kColor, float power)
	{
		m_data.specular = glm::vec4(kColor, power);
	}

	void Material::SetEmissive(const glm::vec3 &kColor)
	{
		m_data.emissive = glm::vec4(kColor, 1.0f);
	}

	void Material::SetAlpha(float alpha)
	{
		m_data.diffuse.a = alpha;
	}
} // End namespace (BGE)
