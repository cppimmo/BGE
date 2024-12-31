/*******************************************************************************
 * @file   EventRegistry.cpp
 * @author Brian Hoffpauir
 * @date   12.10.2024
 * @brief  Definition of EventRegistry class.
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
#include "Events/EventRegistry.hpp"

namespace BGE
{
	EventRegistry::EventRegistry(std::string_view name)
		: m_name(name)
	{
	}

	std::string EventRegistry::VToString(void) const
	{
		using enum EventSpecification;
		std::ostringstream oss;
		std::vector<EventMap::value_type> internalEvents;
		std::vector<EventMap::value_type> externalEvents;

		// Separate events by specification
		for (const auto &[kType, kMetadata] : m_events)
		{
			if (kMetadata.spec == kInternal)
				internalEvents.emplace_back(kType, kMetadata);
			else if (kMetadata.spec == kExternal)
				externalEvents.emplace_back(kType, kMetadata);
		}

		auto log = [&oss](const EventMap::value_type &kPair) mutable
		{
			const auto &[kType, kMetadata] = kPair;
			oss << "Type: 0x" << std::uppercase << std::hex << kType
				<< ", Name: " << kMetadata.name
				<< ", Spec: " << EventSpecToString(kMetadata.spec) << '\n';
		};
		oss << "Registered Engine Events (" << m_name << "):\n";
		std::ranges::for_each(internalEvents, log);

		oss << "Registered Game Events (" << m_name << "):\n";
		std::ranges::for_each(externalEvents, log);
		return oss.str();
	}

	void EventRegistry::RegisterEvent(EventType type, EventSpecification spec, std::string_view name)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_events[type] = {spec, std::string(name)};
	}

	void EventRegistry::LogRegisteredEvents(void) const
	{
		BGE_LOG("Events", "%s", VToString().c_str());
	}

	const std::string &EventRegistry::GetName(void) const
	{
		return m_name;
	}
} // End namespace (BGE)
