/*******************************************************************************
 * @file   ActorFactory.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Declaration of ActorFactory class.
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
