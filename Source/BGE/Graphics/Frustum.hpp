#ifndef _BGE_FRUSTUM_HPP_
#define _BGE_FRUSTUM_HPP_

#include <array>

#include <glm/glm.hpp>

namespace BGE
{
	class Frustum; // Forward declare
	BGE_DECLARE_PTR(Frustum);

	/**
	 * @brief .
	 */
	class Frustum final
	{
	public:
		//! .
		enum Plane
		{
			kPLANE_LEFT,   /**< . */
			kPLANE_RIGHT,  /**< . */
			kPLANE_TOP,    /**< . */
			kPLANE_BOTTOM, /**< . */
			kPLANE_NEAR,   /**< . */
			kPLANE_FAR,    /**< . */
			kPLANE_COUNT   /**< . */
		};
	private:
		std::array<glm::vec4, kPLANE_COUNT> m_planes; //< .
	public:
		Frustum(void) = default;
		Frustum(const Frustum &) = default;
		Frustum(Frustum &&) noexcept = default;
		Frustum &operator=(const Frustum &) = default;
		Frustum &operator=(Frustum &&) noexcept = default;

		void Update(const glm::mat4 &kViewProjMatrix);
		// Accessors:
		bool IsPointInside(const glm::vec3 &kPoint) const;
		bool IsSphereInside(const glm::vec3 &kCenter, float radius) const;
		bool IsAABBInside(const glm::vec3 &kMin, const glm::vec3 &kMax) const;
	private:
		static glm::vec4 ExtractPlane(const glm::mat4 &kMatrix, const glm::vec4 &kNormal);
		static glm::vec3 SelectSupportPoint(const glm::vec4 &kPlane, const glm::vec3 &kMin, const glm::vec3 &kMax);
	};
} // End namespace (BGE)

#endif /* !_BGE_FRUSTUM_HPP_ */
