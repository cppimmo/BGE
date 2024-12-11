/*******************************************************************************
 * @file   EventRegistry.hpp
 * @author Brian Hoffpauir
 * @date   12.10.2024
 * @brief  Declarations of engine events.
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
#ifndef _BGE_EVENTREGISTRY_HPP_
#define _BGE_EVENTREGISTRY_HPP_

#include "Events/EventData.hpp"

namespace BGE
{
	class EventRegistry; // Forward declare
	BGE_DECLARE_PTR(EventRegistry);

	/**
	 * @brief .
	 */
	class EventRegistry final : public INonCopyable, public INonMovable, public IStringable
	{
	public:
		//!
		struct EventMetadata
		{
			EventSpecification spec;
			std::string name;
		};
	private:
		using EventMap = std::map<EventType, EventMetadata>; // Map event types to metadata
		static UniqueEventRegistryPtr s_pInstance;

		mutable std::mutex m_mutex;
		EventMap m_events;
	public:
		~EventRegistry(void) = default; // Public default destructor
		// IStringable's interface:
		virtual std::string VToString(void) const override;
		//!
		void RegisterEvent(EventType type, EventSpecification spec, std::string_view name);
		//!
		void LogRegisteredEvents(void) const;
		// Accessors:
		static EventRegistry &Get(void);
	private:
		EventRegistry(void) = default;  // Private default constructor
	};
} // End namespace (BGE)

// Macros for registering/creating events:
#define BGE_REGISTER_EVENT(EVENT_CLASS) \
	BGE::EventRegistry::Get().RegisterEvent(EVENT_CLASS::kEVENT_TYPE, \
											EVENT_CLASS::kEVENT_SPEC, EVENT_CLASS::kEVENT_NAME) \

#endif /* !_BGE_EVENTREGISTRY_HPP_ */
