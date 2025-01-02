#ifndef _BGE_TRANSFORMCOMPONENT_HPP_
#define _BGE_TRANSFORMCOMPONENT_HPP_

namespace BGE
{
	class Transform; // Forward declare
	BGE_DECLARE_PTR(Transform);

	/**
	 * @brief .
	 */
	class Transform
	{
	private:
		glm::vec3 m_position = glm::vec3(0);
		glm::vec3 m_rotation = glm::vec3(0); // Euler angles
		glm::vec3 m_scale = glm::vec3(1);
		mutable glm::mat4 m_cachedMatrix = glm::mat4(1.0f);
		mutable bool m_bDirty = true;
	public:
		Transform(void) = default;
		Transform(const Transform &) = default;
		Transform(Transform &&) noexcept = default;
		Transform &operator=(const Transform &) = default;
		Transform &operator=(Transform &&) noexcept = default;

		glm::mat4 ToMat4(void) const;
		operator glm::mat4() const { return ToMat4(); }
		// Accessors:
		void SetPosition(const glm::vec3 &kPos);
		void SetRotation(const glm::vec3 &kRot);
		void SetScale(const glm::vec3 &kScl);

		const glm::vec3 &GetPosition(void) const;
		const glm::vec3 &GetRotation(void) const;
		const glm::vec3 &GetScale(void) const;
	private:
		void RecalculateMatrix(void) const;
	};
} // End namespace (BGE)

#endif /* !_BGE_TRANSFORMCOMPONENT_HPP_ */
