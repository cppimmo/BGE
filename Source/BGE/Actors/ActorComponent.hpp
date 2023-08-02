#ifndef _BGE_ACTORCOMPONENT_HPP_
#define _BGE_ACTORCOMPONENT_HPP_

namespace BGE
{
	class ActorComponent
	{
		friend class ActorFactory;
	protected:
		StrongActorPtr m_pOwner;
	public:
		virtual ~ActorComponent(void) { m_pOwner.reset(); }

		// Abstract methods for component implementations:
		virtual bool VInit(tinyxml2::XMLElement *pData) = 0;
		virtual void VPostInit(void) { }
		virtual void VUpdate(float deltaTime) { }
		virtual void VOnChanged(void) { }
		// Editor methods:
		virtual tinyxml2::XMLElement *VGenerateXML(void) = 0;
		// Accessors:
		virtual ActorComponentID VGetID(void) const { return GetIDFromName(VGetName()); }
		virtual std::string VGetName(void) const = 0;
		static ActorComponentID GetIDFromName(std::string_view componentName)
		{
			return 0;
		}
	private:
		void SetOwnerPtr(StrongActorPtr pOwner) { m_pOwner = pOwner; }
	};
} // End namespace (BGE)

#endif /* !_BGE_ACTORCOMPONENT_HPP_ */