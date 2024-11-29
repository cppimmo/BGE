#ifndef _BGE_BASEGAMELOGIC_HPP_
#define _BGE_BASEGAMELOGIC_HPP_

#include "Actors/Actor.hpp"
#include "Actors/ActorFactory.hpp"
#include "MainLoop/ProcessManager.hpp"
#include "Utilities/Timer.hpp"

namespace BGE
{
	enum struct BaseGameState
	{
		kInvalid,
		kInitializing,
		kMainMenu,
		kWaitingForPlayers,
		kLoadingGameEnvironment,
		kWaitingForPlayersToLoadEnvironment,
		kSpawningPlayersActors,
		kRunning
	};

	class IGameLogic
	{
	public:
		virtual ~IGameLogic(void) = default;

		virtual WeakActorPtr VGetActor(ActorID ID) = 0;
		//virtual StrongActorPtr VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4 *initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID)=0;
		virtual void VDestroyActor(ActorID ID) = 0;
		virtual bool VLoadGame(std::string_view levelResource) = 0;
		virtual void VSetProxy(void) = 0;
		virtual void VOnUpdate(float time, float elapsedTime) = 0;
		virtual void VChangeState(BaseGameState state) = 0;
		//virtual void VMoveActor(const ActorId id, Mat4x4 const &mat)=0;
		//virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;
	};

	class BaseGameLogic : public IGameLogic
	{
		friend class EngineApp;
	protected:
		BaseGameState m_state;
		bool m_bProxy;
		Timer m_lifetimeTimer;
		ProcessManager m_processManager;
		ActorFactory m_actorFactory;
	public:
		BaseGameLogic(void);
		virtual ~BaseGameLogic(void);

		bool Init(void);
		// BaseGameLogic interface:
		virtual void VAddView(void);
		virtual void VRemoveView(void);
		// IGameLogic interface:
		virtual WeakActorPtr VGetActor(ActorID ID);
		virtual void VDestroyActor(ActorID ID);
		virtual bool VLoadGame(std::string_view levelResource);
		virtual void VSetProxy(void);
		virtual void VOnUpdate(float time, float elapsedTime);
		virtual void VChangeState(BaseGameState state);
		// Accessors:
		ProcessManager &GetProcessManager(void) noexcept;
		const ProcessManager &GetProcessManager(void) const noexcept;
		bool IsProxy(void) const;
		void SetProxy(bool bProxy) noexcept;
	private:
	};
} // End namespace (BGE)

#endif /* !_BGE_BASEGAMELOGIC_HPP_ */
