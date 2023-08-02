#ifndef _BGE_ACTOR_HPP_
#define _BGE_ACTOR_HPP_

namespace BGE
{
	/**
	 *
	 */
	class Actor
	{
		friend class ActorFactory;
	public:
		using ActorComponentMap = std::map<ActorComponentID, StrongActorComponentPtr>;
	private:
		ActorID m_ID; // Unique ID for this actor
		ActorComponentMap m_components; // All components of this actor
		ActorType m_type; // Name of this actor
		std::string m_resourceFilename; // name of XML init file
	public:
		explicit Actor(ActorID aID);
		~Actor(void);

		bool Init(tinyxml2::XMLElement *pData);
		void PostInit(void);
		void Destroy(void);
		void Update(float deltaTime);
		// Editor methods:
		std::string ToXML(void);
		// Accessors:
		ActorID GetID(void) const noexcept { return m_ID; }
		ActorType GetType(void) const noexcept { return m_type; }
		// Template methods for accessing components:
		template <typename ActorComponentType>
		std::weak_ptr<ActorComponentType> GetComponentPtr(ActorComponentID cID)
		{
			auto findIter = m_components.find(cID);
			if (findIter != m_components.end())
			{
				StrongActorComponentPtr pBase(findIter->second);
				// Cast to subclass version of the pointer
				std::shared_ptr<ActorComponentType> pSub(std::static_pointer_cast<ActorComponentType>(pBase));
				// Convert strong pointer to weak pointer
				std::weak_ptr<ActorComponentType> pWeakSub(pSub);
				return pWeakSub; // Return the weak pointer
			}
			else
				return std::weak_ptr<ActorComponentType>(); // No component found
		}
		
		template <typename ActorComponentType>
		std::weak_ptr<ActorComponentType> GetComponent(std::string_view componentName)
		{
			ActorComponentID aID = ActorComponent::GetIDFromName(componentName);
			auto findIter = m_components.find(aID);
			if (findIter != m_components.end())
			{
				StrongActorComponentPtr pBase(findIter->second);
				// Cast to subclass version of the pointer
				std::shared_ptr<ActorComponentType> pSub(std::static_pointer_cast<ActorComponentType>(pBase));
				// Convert strong pointer to weak pointer
				std::weak_ptr<ActorComponentType> pWeakSub(pSub);
				return pWeakSub; // Return the weak pointer
			}
			else
				return std::weak_ptr<ActorComponentType>(); // No component found
		}
		const ActorComponentMap &GetComponents(void) const { return m_components; }
		void AddComponent(StrongActorComponentPtr pComponent);
	};
} // End namespace (BGE)

#endif /* !_BGE_ACTOR_HPP_ */
