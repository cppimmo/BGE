/*******************************************************************************
 * @file   EventManager.hpp
 * @author Brian Hoffpauir
 * @date   12.08.2024
 * @brief  Declaration of IEventManager interface.
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
#ifndef _BGE_EVENTMANAGER_HPP_
#define _BGE_EVENTMANAGER_HPP_

#include "Events/EventData.hpp"

namespace BGE
{
	class IEventManager; // Forward declare
	BGE_DECLARE_PTR(IEventManager);

	class IEventData; // Forware declare

	/**
	 * @brief .
	 */
	class IEventManager
	{
	public:
		static constexpr float kINFINITY = std::numeric_limits<float>::infinity();
	public:
		virtual ~IEventManager(void) = default;

		virtual bool VAddListener(const EventListenerDelegate &kDelegate, const EventType &kType) = 0;

		virtual bool VRemoveListener(const EventListenerDelegate &kDelegate, const EventType &kType) = 0;

		virtual bool VTriggerEvent(const StrongIEventDataPtr &pEvent) const = 0;

		virtual bool VQueueEvent(const StrongIEventDataPtr &pEvent) = 0;

		virtual bool VThreadSafeQueueEvent(const StrongIEventDataPtr &pEvent) = 0;

		virtual bool VAbortEvent(const EventType &kType, bool bAllOfType = false) = 0;

		virtual bool VUpdate(float maxDeltaTime = kINFINITY) = 0;
	};
} // End namespace (BGE)

#include "Events/EventManagerImpl.hpp" // Concrete event manager implementation

#endif /* !_BGE_EVENTMANAGER_HPP_ */
