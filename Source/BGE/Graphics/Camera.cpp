#include "Engine/EngineStd.hpp"
#include "Graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace BGE
{
	Camera::Camera(CameraType type, const ProjectionDesc &kDesc)
		: m_type(type),
		  m_position(0.0f, 0.0f, 0.0f),
		  m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		  m_projDesc(kDesc)
	{
	}

	void Camera::LookAt(const glm::vec3 &kTarget, const glm::vec3 &kUp)
	{
		glm::mat4 viewMatrix = glm::lookAt(m_position, kTarget, kUp);
		m_orientation = glm::quat_cast(glm::inverse(viewMatrix));
		m_bViewDirty = true;
	}

	const glm::vec3 &Camera::GetPosition(void) const
	{
		return m_position;
	}

	const glm::quat &Camera::GetOrientation(void) const
	{
		return m_orientation;
	}

	const glm::mat4 &Camera::GetViewMatrix(void) const
	{
		if (m_bViewDirty)
		{
			UpdateViewMatrix();
			m_frustum.Update(m_projectionMatrix * m_viewMatrix);
		}

		return m_viewMatrix;
	}

	const glm::mat4 &Camera::GetProjectionMatrix(void) const
	{
		if (m_bProjectionDirty)
		{
			UpdateProjectionMatrix();
			m_frustum.Update(m_projectionMatrix * m_viewMatrix);
		}

		return m_projectionMatrix;
	}

	const ProjectionDesc &Camera::GetProjectionDesc(void) const
	{
		return m_projDesc;
	}

	void Camera::SetPosition(const glm::vec3 &kPosition)
	{
		m_position = kPosition;
		m_bViewDirty = true;
	}

	void Camera::SetOrientation(const glm::quat &kOrientation)
	{
		m_orientation = kOrientation;
		m_bViewDirty = true;
	}

	void Camera::SetPerspective(const ProjectionDesc &kDesc)
	{
		m_type = CameraType::kPerspective;
		m_projDesc = kDesc;
		m_bProjectionDirty = true;
	}

	void Camera::SetOrthographic(const Math::RectFloat &kRect, float nearClip, float farClip)
	{
		m_type = CameraType::kOrthographic;
		m_orthoRect = kRect;
		m_projDesc.nearClip = nearClip;
		m_projDesc.farClip = farClip;
		m_bProjectionDirty = true;
	}

	void Camera::UpdateViewMatrix(void) const
	{
		m_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_orientation));
		m_bViewDirty = false;
	}

	void Camera::UpdateProjectionMatrix(void) const
	{
		if (m_type == CameraType::kPerspective)
		{
			m_projectionMatrix = glm::perspective(
				glm::radians(m_projDesc.fieldOfView),
				m_projDesc.aspectRatio, m_projDesc.nearClip, m_projDesc.farClip);
		}
		else if (m_type == CameraType::kOrthographic)
		{
			m_projectionMatrix = glm::ortho(m_orthoRect.left, m_orthoRect.right, m_orthoRect.bottom,
				m_orthoRect.top, m_projDesc.nearClip, m_projDesc.farClip);
		}

		m_bProjectionDirty = false;
	}
} // End namespace (BGE)
