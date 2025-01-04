#include "Engine/EngineStd.hpp"
#include "Graphics/Frustum.hpp"

namespace BGE
{
	void Frustum::Update(const glm::mat4 &kViewProjMatrix)
	{
		m_planes[kPLANE_LEFT] = ExtractPlane(kViewProjMatrix, { 1, 0, 0, 1 }); // Left
		m_planes[kPLANE_RIGHT] = ExtractPlane(kViewProjMatrix, { -1, 0, 0, 1 }); // Right
		m_planes[kPLANE_TOP] = ExtractPlane(kViewProjMatrix, { 0, -1, 0, 1 }); // Top
		m_planes[kPLANE_BOTTOM] = ExtractPlane(kViewProjMatrix, { 0, 1, 0, 1 }); // Bottom
		m_planes[kPLANE_NEAR] = ExtractPlane(kViewProjMatrix, { 0, 0, 0, 1 }); // Near
		m_planes[kPLANE_FAR] = ExtractPlane(kViewProjMatrix, { 0, 0, -1, 1 }); // Far
	}

	bool Frustum::IsPointInside(const glm::vec3 &kPoint) const
	{
		for (const auto &kPlane : m_planes)
		{
			if ((glm::dot(glm::vec3(kPlane), kPoint) + kPlane.w) < 0)
				return false; // Outside of the frustum
		}
		return true; // Inside the frustum
	}

	bool Frustum::IsSphereInside(const glm::vec3 &kCenter, float radius) const
	{
		for (const auto &kPlane : m_planes)
		{
			if ((glm::dot(glm::vec3(kPlane), kCenter) + kPlane.w) < -radius)
				return false; // Outside of the frustum
		}
		return true; // Inside the frustum
	}

	bool Frustum::IsAABBInside(const glm::vec3 &kMin, const glm::vec3 &kMax) const
	{
		for (const auto &kPlane : m_planes)
		{
			const glm::vec3 kCorner = SelectSupportPoint(kPlane, kMin, kMax);
			if ((glm::dot(glm::vec3(kPlane), kCorner) + kPlane.w) < 0)
				return false; // Outside of the frustum
		}
		return true; // Inside the frustum
	}

	glm::vec4 Frustum::ExtractPlane(const glm::mat4 &kMatrix, const glm::vec4 &kNormal)
	{
		const glm::vec4 kPlane = glm::transpose(kMatrix) * kNormal;
		const float kMagnitude = glm::length(glm::vec3(kPlane));
		return kPlane / kMagnitude; // Normalize the plane
	}

	glm::vec3 Frustum::SelectSupportPoint(const glm::vec4 &kPlane, const glm::vec3 &kMin, const glm::vec3 &kMax)
	{
		glm::vec3 supportPoint;
		supportPoint.x = (kPlane.x < 0) ? kMin.x : kMax.x;
		supportPoint.y = (kPlane.y < 0) ? kMin.y : kMax.y;
		supportPoint.z = (kPlane.z < 0) ? kMin.z : kMax.z;
		return supportPoint;
	}
} // End namespace (BGE)
