/*******************************************************************************
 * @file   ActorComponent.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Declaration of ActorComponent class.
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
