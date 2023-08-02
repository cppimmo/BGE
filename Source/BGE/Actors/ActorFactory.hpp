#ifndef _BGE_ACTORFACTORY_HPP_
#define _BGE_ACTORFACTORY_HPP_

namespace BGE
{
	class ActorFactory
	{
		ActorID m_lastActorID; // ID of the last constructed Actor
	protected:
		// TODO: 
	public:
		ActorFactory(void);

		StrongActorPtr CreateActor(std::string_view xmlFilename, tinyxml2::XMLElement *pOverrides, const Math::Mat4x4f &initialTransform, ActorID serverActorID);
		void ModifyActor(StrongActorPtr pActor, tinyxml2::XMLElement *pOverrides);
		virtual StrongActorComponentPtr VCreateComponent(tinyxml2::XMLElement *pData);
	private:
		// Increment internal ID count and return the previous ID.
		ActorID GetNextActorID(void) { ++m_lastActorID; return m_lastActorID; }
	};
} // End namespace (BGE)

#endif /* !_BGE_ACTORFACTORY_HPP_ */