/*******************************************************************************
 * @file   EventData.hpp
 * @author Brian Hoffpauir
 * @date   12.08.2024
 * @brief  .
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
#ifndef _BGE_EVENTDATA_HPP_
#define _BGE_EVENTDATA_HPP_

#include <FastDelegate.hpp> // Used for delegates
#include <cstddef>

namespace BGE
{
	class IEventData; // Forware declare
	BGE_DECLARE_PTR(IEventData);

	//! Manually set unique ID for all events.
	using EventType = std::uint64_t;
	//! Event listener delegate.
	using EventListenerDelegate = fastdelegate::FastDelegate<void(StrongIEventDataPtr)>;

	/**
	 * @brief .
	 */
	class IEventData
	{
	public:
		virtual ~IEventData(void) = default;
		//!
		virtual const EventType &VGetEventType(void) const = 0;
		//!
		virtual const Timestamp &VGetTimestamp(void) const = 0;
		//!
		virtual void VSerialize(std::ostringstream &oss) const = 0;
		//!
		virtual void VDeserialize(std::istringstream &iss) = 0;
		//!
		virtual StrongIEventDataPtr VCopy(void) const = 0;
		//!
		virtual constexpr std::string_view VGetName(void) const = 0;
	};

	/**
	 * @brief .
	 */
	class BaseEventData : public IEventData
	{
		Timestamp m_timestamp; //!< Time the event was created
	public:
		explicit BaseEventData(Timestamp timestamp = std::chrono::system_clock::now());
		virtual ~BaseEventData(void) = default;

		// IEventData's interface:
		// NOTE: VGetEventType is left as pure virtual.
		//!
		virtual const Timestamp &VGetTimestamp(void) const override final;
		//!
		virtual void VSerialize(std::ostringstream &oss) const override;
		virtual void VDeserialize(std::istringstream &iss) override;
	};
} // End namespace (BGE)

// Macros for registering/creating events:
#define BGE_REGISTER_EVENT(EVENT_CLASS) 0
#define BGE_CREATE_EVENT(EVENT_TYPE) 0

#endif /* !_BGE_EVENTDATA_HPP_ */
