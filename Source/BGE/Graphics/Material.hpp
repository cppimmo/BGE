#ifndef _BGE_MATERIAL_HPP_
#define _BGE_MATERIAL_HPP_

#include "Utilities/Math.hpp"

namespace BGE
{
	class Material; // Forward declare
	BGE_DECLARE_PTR(Material);

	//! Template color type based on glm::vec.
	template <Math::Numeric Type>
	using Color = glm::vec<4, Type, glm::defaultp>;

	//! Colors with float underlying type.
	using ColorFloat = Color<float>;
	//! Colors with unsigned char (ubyte) underlying type.
	using ColorUByte = Color<unsigned char>;

	inline constexpr float kALPHA_OPAQUE = 1.0f;
	inline constexpr float kALPHA_TRANSPARENT = 0.0f;

	// Common colors with RGBA values.
	inline constexpr Color kCOLOR_BLACK       = ColorFloat(0.0f, 0.0f, 0.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_RED         = ColorFloat(1.0f, 0.0f, 0.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_GREEN       = ColorFloat(0.0f, 1.0f, 0.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_BLUE        = ColorFloat(0.0f, 0.0f, 1.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_YELLOW      = ColorFloat(1.0f, 1.0f, 0.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_CYAN        = ColorFloat(0.0f, 1.0f, 1.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_MAGENTA     = ColorFloat(1.0f, 0.0f, 1.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_GRAY        = ColorFloat(0.5f, 0.5f, 0.5f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_LIGHT_GRAY  = ColorFloat(0.75f, 0.75f, 0.75f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_DARK_GRAY   = ColorFloat(0.25f, 0.25f, 0.25f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_ORANGE      = ColorFloat(1.0f, 0.65f, 0.0f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_PURPLE      = ColorFloat(0.5f, 0.0f, 0.5f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_PINK        = ColorFloat(1.0f, 0.75f, 0.8f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_BROWN       = ColorFloat(0.65f, 0.16f, 0.16f, kALPHA_OPAQUE);
	inline constexpr Color kCOLOR_GOLD        = ColorFloat(1.0f, 0.84f, 0.0f, kALPHA_OPAQUE);

	// Transparent variants
	inline constexpr Color kCOLOR_TRANSPARENT_BLACK = ColorFloat(0.0f, 0.0f, 0.0f, kALPHA_TRANSPARENT);
	inline constexpr Color kCOLOR_TRANSPARENT_WHITE = ColorFloat(1.0f, 1.0f, 1.0f, kALPHA_TRANSPARENT);
	inline constexpr Color kCOLOR_TRANSPARENT_RED   = ColorFloat(1.0f, 0.0f, 0.0f, kALPHA_TRANSPARENT);
	inline constexpr Color kCOLOR_TRANSPARENT_BLUE  = ColorFloat(0.0f, 0.0f, 1.0f, kALPHA_TRANSPARENT);
	inline constexpr Color kCOLOR_TRANSPARENT_GREEN = ColorFloat(0.0f, 1.0f, 0.0f, kALPHA_TRANSPARENT);

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
