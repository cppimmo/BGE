#ifndef _BGE_CAMERA_HPP_
#define _BGE_CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Utilities/Math.hpp"
#include "Graphics/Frustum.hpp"

namespace BGE
{
	class Camera; // Forward declare
	BGE_DECLARE_PTR(Camera);
	class FirstPersonCamera; // Forward declare
	BGE_DECLARE_PTR(FirstPersonCamera);

	//! .
	enum struct CameraType
	{
		kPerspective, /**< . */
		kOrthographic /**< . */
	};

	//! .
	struct ProjectionDesc
	{
		float fieldOfView; //!< .
		float aspectRatio; //!< .
		float nearClip; //!< .
		float farClip; //!< .
	};

	/**
	 * @brief .
	 */
	class Camera
	{
	protected:
		CameraType m_type; //!< .
		glm::vec3 m_position; //!< .
		glm::quat m_orientation; //!< .

		ProjectionDesc m_projDesc; //!< .
		mutable Frustum m_frustum; //!< .
		// Orthographic parameters
		Math::RectFloat m_orthoRect = { -1.0f, 1.0f, -1.0f, 1.0f };

		mutable glm::mat4 m_viewMatrix; //!< .
		mutable glm::mat4 m_projectionMatrix; //!< .

		mutable bool m_bViewDirty = true; //!< .
		mutable bool m_bProjectionDirty = true; //!< .
	public:
		Camera(CameraType type, const ProjectionDesc &kDesc);
		virtual ~Camera(void) = default;

		void LookAt(const glm::vec3 &kTarget, const glm::vec3 &kUp);
		// Accessors:
		const glm::vec3 &GetPosition(void) const;
		const glm::quat &GetOrientation(void) const;
		const glm::mat4 &GetViewMatrix(void) const;
		const glm::mat4 &GetProjectionMatrix(void) const;
		const ProjectionDesc &GetProjectionDesc(void) const;
		const Frustum &GetFrustum(void) const;

		void SetPosition(const glm::vec3 &kPosition);
		void SetOrientation(const glm::quat &kOrientation);
		void SetPerspective(const ProjectionDesc &kDesc);
		void SetOrthographic(const Math::RectFloat &kRect, float nearClip, float farClip);
	protected:
		void UpdateViewMatrix(void) const;
		void UpdateProjectionMatrix(void) const;
	};

	/**
	 * @brief .
	 */
	class FirstPersonCamera : public Camera
	{
	public:
		FirstPersonCamera(const ProjectionDesc &kDesc)
			: Camera(CameraType::kPerspective, kDesc)
		{
		}

		void MoveForward(float distance)
		{
			glm::vec3 forward = glm::normalize(m_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
			SetPosition(GetPosition() + forward * distance);
		}

		void MoveUp(float distance)
		{
			glm::vec3 up = glm::normalize(m_orientation * glm::vec3(0.0f, 1.0f, 0.0f));
			SetPosition(GetPosition() + up * distance);
		}

		void MoveRight(float distance)
		{
			glm::vec3 right = glm::normalize(m_orientation * glm::vec3(1.0f, 0.0f, 0.0f));
			SetPosition(GetPosition() + right * distance);
		}

		void Rotate(float yaw, float pitch)
		{
			glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat newOrientation = yawQuat * pitchQuat;
			SetOrientation(newOrientation);
		}
	};
} // End namespace (BGE)

#endif /* !_BGE_CAMERA_HPP_ */
