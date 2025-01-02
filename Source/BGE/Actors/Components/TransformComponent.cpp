#include "Engine/EngineStd.hpp"
#include "Actors/Components/TransformComponent.hpp"

#include <glm/gtc/quaternion.hpp>

namespace BGE
{
	glm::mat4 Transform::ToMat4(void) const
	{
		if (m_bDirty)
		{
			RecalculateMatrix();
		}
		return m_cachedMatrix;
	}

	void Transform::SetPosition(const glm::vec3 &kPos)
	{
		m_position = kPos;
		m_bDirty = true;
	}

	void Transform::SetRotation(const glm::vec3 &kRot)
	{
		m_rotation = kRot;
		m_bDirty = true;
	}

	void Transform::SetScale(const glm::vec3 &kScl)
	{
		m_scale = kScl;
		m_bDirty = true;

	}

	const glm::vec3 &Transform::GetPosition(void) const
	{
		return m_position;
	}

	const glm::vec3 &Transform::GetRotation(void) const
	{
		return m_rotation;
	}

	const glm::vec3 &Transform::GetScale(void) const
	{
		return m_scale;
	}

	void Transform::RecalculateMatrix(void) const
	{
		m_cachedMatrix = glm::translate(glm::mat4(1.0f), m_position);
		m_cachedMatrix *= glm::mat4_cast(glm::quat(m_rotation));
		m_cachedMatrix = glm::scale(m_cachedMatrix, m_scale);
		m_bDirty = false;
	}
} // End namespace (BGE)
