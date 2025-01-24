#ifndef _BGE_SCENEGRAPH_HPP_
#define _BGE_SCENEGRAPH_HPP_

#include <stack>

#include "Actors/Actor.hpp"

namespace BGE
{
	class IRenderer; // Forward declare
	class ISceneNode; // Forward declare
	class CameraNode; // Forward declare
	class IEventData; // Forward declare
	BGE_DECLARE_PTR(IRenderer);
	BGE_DECLARE_PTR(ISceneNode);
	BGE_DECLARE_PTR(CameraNode);
	BGE_DECLARE_PTR(IEventData);

	/**
	 * @brief .
	 */
	class SceneGraph final : public INonCopyable, public INonMovable
	{
		using MatrixStack = std::stack<glm::mat4>;
	private:
		StrongISceneNodePtr m_pRootNode;
		StrongCameraNodePtr m_pCameraNode;
		StrongIRendererPtr m_pRenderer;

		MatrixStack m_matrixStack;
	public:
		SceneGraph(StrongIRendererPtr pRenderer);
		~SceneGraph(void);

		void OnRender(float deltaTime);
		void OnRestore(void);
		void OnLostDevice(void);
		void OnUpdate(float deltaTime);

		StrongISceneNodePtr FindActor(ActorID actorID);
		bool AddChild(ActorID actorID, StrongISceneNodePtr pChild);
		bool RemoveChild(ActorID actorID);

		void SetCamera(StrongCameraNodePtr pCamera) noexcept;
		StrongCameraNodePtr GetCamera(void) const noexcept;

		void PushMatrix(const glm::mat4 &kToWorld);
		glm::mat4 PopMatrix(void);
		glm::mat4 GetTopMatrix(void) const noexcept;


		StrongIRendererPtr GetRenderer(void);
	private:
		// Scene graph event delegates:
		void NewRenderComponentDelegate(StrongIEventDataPtr pEventData);
		void ModifiedRenderComponentDelegate(StrongIEventDataPtr pEventData);
		void DestroyActorDelegate(StrongIEventDataPtr pEventData);
		void MoveActorDelegate(StrongIEventDataPtr pEventData);
	};
} // End namespace (BGE)

#endif /* !_BGE_SCENEGRAPH_HPP_ */
