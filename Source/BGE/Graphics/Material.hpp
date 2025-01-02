#ifndef _BGE_MATERIAL_HPP_
#define _BGE_MATERIAL_HPP_

namespace BGE
{
	class Material; // Forward declare
	BGE_DECLARE_PTR(Material);

	/*
struct Material {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular; // a specular.a stores shininess
    vec4 emissive;
};

uniform Material material;

void main() {
    vec4 finalColor = material.ambient + material.diffuse * texture(textureSampler, texCoords);
    // Add specular and emissive calculations as needed.
}
*/
	/**
	 * @brief .
	 */
	class Material
	{
	public:
		//! .
		struct Data
		{
			glm::vec4 diffuse = glm::vec4(1.0f);   // RGBA for diffuse color
			glm::vec4 ambient = glm::vec4(0.2f);  // Default ambient color
			glm::vec4 specular = glm::vec4(1.0f); // RGB for specular color + Alpha for shininess
			glm::vec4 emissive = glm::vec4(0.0f); // Emissive color
		};
	private:
		Data m_data; //!< .
	public:
		Material(void) = default;
		explicit Material(const Data &kData);
		// Accessors:
		glm::vec3 GetAmbient(void) const;
		glm::vec3 GetDiffuse(void) const;
		void GetSpecular(glm::vec3 &color, float &power) const;
		glm::vec3 GetEmissive(void) const;
		float GetAlpha(void) const;
		bool HasAlpha(void) const;
		const Data &GetData(void) const;

		void SetAmbient(const glm::vec3 &kColor);
		void SetDiffuse(const glm::vec3 &kColor);
		void SetSpecular(const glm::vec3 &kColor, float power);
		void SetEmissive(const glm::vec3 &kColor);
		void SetAlpha(float alpha);
		// TODO: Add member function that sets shader program uniform values.
	};
} // End namespace (BGE)

#endif /* !_BGE_MATERIAL_HPP_ */
