/*******************************************************************************
 * @file   EventManagerImpl.hpp
 * @author Brian Hoffpauir
 * @date   12.08.2024
 * @brief  Declaration of event manager implmentation.
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
#ifndef _BGE_EVENTMANAGERIMPL_HPP_
#define _BGE_EVENTMANAGERIMPL_HPP_

#include "Events/EventData.hpp"
#include "Events/EventManager.hpp"
#include "Engine/EngineApp.hpp"

#include <array>

namespace BGE
{
	class IEventManager; // Forward declare
	class EventManager; // Forward declare
	BGE_DECLARE_PTR(EventManager);

	class IEventData; // Forware declare

	/**
	 * @brief .
	 */
	class EventManager : public IEventManager, public INonCopyable, public INonMovable
	{
	protected:
		using EventListenerList = std::list<EventListenerDelegate>;
		using EventListenerMap = std::map<EventType, EventListenerList>;
		using EventQueue = std::list<StrongIEventDataPtr>;

		static constexpr std::size_t kEVENT_QUEUE_COUNT = 2;

		std::string m_name; //!< Name of the event manager instance
		std::size_t m_queueIndex; //!< Index of currently processing queue
		EventListenerMap m_eventListeners;
		std::array<EventQueue, kEVENT_QUEUE_COUNT> m_queues;
		// TODO: Implement ThreadSafeEventQueue.
	public:
		explicit EventManager(std::string_view name);
		virtual ~EventManager(void) = default;

		// IEventManager's interface:
		virtual bool VAddListener(const EventListenerDelegate &kDelegate, const EventType &kType) override;
		virtual bool VRemoveListener(const EventListenerDelegate &kDelegate, const EventType &kType) override;
		virtual bool VTriggerEvent(const StrongIEventDataPtr &pEvent) const override;
		virtual bool VQueueEvent(const StrongIEventDataPtr &pEvent) override;
		virtual bool VThreadSafeQueueEvent(const StrongIEventDataPtr &pEvent) override;
		virtual bool VAbortEvent(const EventType &kType, bool bAllOfType = false) override;
		virtual bool VUpdate(float maxDeltaTime = kINFINITY) override;
		// Accessors:
		const std::string &GetName(void) const;
	};
} // End namespace (BGE)

#define BGE_ADD_GEVENT_LISTENER(P_CLASS, P_FUNC, EVENT_CLASS) \
{ \
	auto &eventManager = BGE::GetEngineApp().GetEventManager(); \
	eventManager.VAddListener(fastdelegate::MakeDelegate(P_CLASS, P_FUNC), EVENT_CLASS::kEVENT_TYPE); \
} \

#define BGE_REMOVE_GEVENT_LISTENER(P_CLASS, P_FUNC, EVENT_CLASS) \
{ \
	auto &eventManager = BGE::GetEngineApp().GetEventManager(); \
	eventManager.VRemoveListener(fastdelegate::MakeDelegate(P_CLASS, P_FUNC), EVENT_CLASS::kEVENT_TYPE); \
} \

/**
 * The leading G in event stands for global.
 */
#define BGE_QUEUE_GEVENT(P_EVENT) \
{ \
	auto &eventManager = BGE::GetEngineApp().GetEventManager(); \
	eventManager.VQueueEvent(std::static_pointer_cast<BGE::IEventData>(P_EVENT)); \
} \

/**
 *
 */
#define BGE_TRIGGER_GEVENT(P_EVENT) \
{ \
	auto &eventManager = BGE::GetEngineApp().GetEventManager(); \
	eventManager.VTriggerEvent(std::static_pointer_cast<BGE::IEventData>(P_EVENT)); \
} \

#endif /* !_BGE_EVENTMANAGERIMPL_HPP_ */
