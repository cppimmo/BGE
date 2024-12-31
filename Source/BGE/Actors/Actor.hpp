/*******************************************************************************
 * @file   Actor.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Declaration of Actor class.
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#ifndef _BGE_ACTOR_HPP_
#define _BGE_ACTOR_HPP_

#include "Actors/ActorComponent.hpp"

namespace BGE
{
	class Actor; // Forward declare
	using ActorMap = std::map<ActorID, StrongActorPtr>;
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
