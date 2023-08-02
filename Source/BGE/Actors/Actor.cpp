#include "Engine/EngineStd.hpp"
#include "Actors/Actor.hpp"
#include "Actors/ActorComponent.hpp"

BGE::Actor::Actor(ActorID aID)
{
}

BGE::Actor::~Actor(void)
{
}

bool BGE::Actor::Init(tinyxml2::XMLElement *pData)
{
	return false;
}

void BGE::Actor::PostInit(void)
{
	for (auto &[key, value] : m_components)
	{

	}
}

void BGE::Actor::Destroy(void)
{
	m_components.clear();
}

void BGE::Actor::Update(float deltaTime)
{
	for (auto &[key, value] : m_components)
	{

	}
}

std::string BGE::Actor::ToXML(void)
{
	return std::string();
}

void BGE::Actor::AddComponent(StrongActorComponentPtr pComponent)
{
}
