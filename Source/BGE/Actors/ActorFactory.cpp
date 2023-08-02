#include "Engine/EngineStd.hpp"
#include "ActorFactory.hpp"
#include "ActorComponent.hpp"

BGE::ActorFactory::ActorFactory(void)
	: m_lastActorID(kINVALID_ACTOR_ID)
{
}

BGE::StrongActorComponentPtr BGE::ActorFactory::VCreateComponent(tinyxml2::XMLElement *pData)
{
	return StrongActorComponentPtr();
}