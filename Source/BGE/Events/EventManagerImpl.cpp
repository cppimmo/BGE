/*******************************************************************************
 * @file   EventManagerImpl.cpp
 * @author Brian Hoffpauir
 * @date   12.08.2024
 * @brief  Definition of event manager implmentation.
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
#include "Engine/EngineStd.hpp"
#include "Events/EventManagerImpl.hpp"

#include <ranges>

namespace BGE
{
	EventManager::EventManager(std::string_view name)
		: m_name(name), m_queueIndex(0)
	{
	}

	bool EventManager::VAddListener(const EventListenerDelegate &kDelegate, const EventType &kType)
	{
		BGE_LOG("Events", "Attempting to add delegate function for event type: 0x%X", kType);

		// Find or create the entry for the given event type
		auto &listeners = m_eventListeners[kType];

		// Check for duplicated delegates
		if (std::ranges::find(listeners, kDelegate) != listeners.end())
		{
			BGE_WARNING("Attempting to double-register a delegate");
			return false;
		}

		// Add the delegate
		listeners.push_back(kDelegate);
		BGE_LOG("Events", "Successfully added delegate for event type: 0x%X", kType);
		return true;
	}

	bool EventManager::VRemoveListener(const EventListenerDelegate &kDelegate, const EventType& kType)
	{
		BGE_LOG("Events", "Attempting to remove delegate function from event type: 0x%X", kType);

		// Check if the event type exists
		if (!m_eventListeners.contains(kType))
		{
			return false;
		}

		auto &listeners = m_eventListeners[kType];
		/*
		 * Remove delegates from the listener that match the input delegate. FastDelegate has an
		 * overloaded == operator which makes this possible.
		 */
		const auto kRemovedCount = std::erase_if(
			listeners,
			[&kDelegate](const EventListenerDelegate &currDelegate) { return kDelegate == currDelegate; }
		);

		// If elements were removed, erase them
		if (kRemovedCount > 0)
		{
			BGE_LOG("Events", "Successfully removed delegate function from event type: 0x%X", kType);
			return true;
		}
		return false; // No delegates were removed
	}

	bool EventManager::VTriggerEvent(const StrongIEventDataPtr &pEvent) const
	{
		BGE_LOG("Events", "Attempting to trigger event: %s", pEvent->VGetName().data());

		bool bProcessed = false;
		const auto kType = pEvent->VGetEventType();

		auto findIt = m_eventListeners.find(kType);
		// Check if the event type exists
		if (findIt != m_eventListeners.end())
		{
			auto &listeners = findIt->second;
			// Invoke each delegate that matches the event type with argument pEvent
			for (const auto &listener : listeners)
			{
				BGE_LOG("Events", "Sending event %s to delegate...", pEvent->VGetName().data());
				listener(pEvent); // Call the delegate
				bProcessed = true;
			}
		}
		return bProcessed;
	}

	bool EventManager::VQueueEvent(const StrongIEventDataPtr &pEvent)
	{
		// Ensure queue index is valid
		BGE_ASSERT(m_queueIndex >= 0);
		BGE_ASSERT(m_queueIndex < kEVENT_QUEUE_COUNT);

		// Ensure the event is valid
		if (!pEvent)
		{
			BGE_ERROR("Invalid event in VQueueEvent()");
			return false;
		}

		BGE_LOG("Events", "Attempting to queue event: %s", pEvent->VGetName().data());

		auto findIt = m_eventListeners.find(pEvent->VGetEventType());
		if (findIt != m_eventListeners.end())
		{
			m_queues[m_queueIndex].push_back(pEvent);
			BGE_LOG("Events", "Successfully queue event: %s", pEvent->VGetName().data());
			return true;
		}
		BGE_LOG("Events", "Skipping event since there are no delegates registered to receive it: %s", pEvent->VGetName().data());
		return false;
	}

	bool EventManager::VThreadSafeQueueEvent(const StrongIEventDataPtr &pEvent)
	{
		BGE_ASSERT(false && "Operation not implemented");

		return true;
	}

	bool BGE::EventManager::VAbortEvent(const EventType& kType, bool bAllOfType)
	{
		BGE_ASSERT(m_queueIndex >= 0);
		BGE_ASSERT(m_queueIndex < kEVENT_QUEUE_COUNT);

		bool bSuccess = false;
		if (m_eventListeners.contains(kType))
		{
			auto &queue = m_queues[m_queueIndex];

			// Remove one or all events from the queue of the given type
			const auto kRemovedCount = std::erase_if(queue, [&](const auto &kEvent)
				{
					bool bShouldRemove = (kEvent->VGetEventType() == kType);
					if (bShouldRemove)
					{
						bSuccess = true;
						// If not removing all events, return true for just the first match
						if (!bAllOfType)
							return true;
					}
					return bAllOfType; // Remove if allOfType is true, otherwise just the first match
				});
			// Erase stops after one if not allOfType, so it won't remove unnecessarily.
		}
		return bSuccess;
	}

	bool EventManager::VUpdate(float maxDeltaTime)
	{
		// TODO: Add timeout check variables.

		// TODO: Add threaded event handling.

		// Swap active queues and clear the new queue after the swap
		const auto kCurrIndex = m_queueIndex;
		m_queueIndex = (m_queueIndex + 1) % kEVENT_QUEUE_COUNT;
		m_queues[m_queueIndex].clear();

		// Process the queue
		while (!m_queues[kCurrIndex].empty())
		{
			// Pop the front of the queue
			auto pEvent = m_queues[kCurrIndex].front();
			m_queues[kCurrIndex].pop_front();
			BGE_LOG("EventLoop", "\tProcessing event: %s", pEvent->VGetName().data());

			const auto kType = pEvent->VGetEventType();

			// Find all the delegate functions registered for this event
			auto findIt = m_eventListeners.find(kType);
			if (findIt != m_eventListeners.end())
			{
				const auto &listeners = findIt->second;
				BGE_LOG("EventLoop", "\tFound %llu delegates", listeners.size());

				// Invoke each listener
				for (const auto &listener : listeners)
				{
					BGE_LOG("EventLoop", "\tSending event %s to delegate", pEvent->VGetName().data());
					listener(pEvent);
				}
			}

			// TODO: Perform timeout check
			if (false)
			{
				BGE_LOG("EventLoop", "Aborting event processing; time ran out");
				break;
			}
		}

		/*
		 * Push remaining events to the new active queue, if not all were processed.
		 * NOTE: Inserting at head of active queue (back-to-front) to preserve sequencing.
		 */
		const bool kbQueueFlushed = m_queues[kCurrIndex].empty();
		if (!kbQueueFlushed)
		{
			while (!m_queues[kCurrIndex].empty())
			{
				auto pEvent = m_queues[kCurrIndex].back();
				m_queues[kCurrIndex].pop_back();
				m_queues[m_queueIndex].push_front(pEvent);
			}
		}
		return true;
	}

	const std::string &EventManager::GetName(void) const
	{
		return m_name;
	}
} // End namespace (BGE)
