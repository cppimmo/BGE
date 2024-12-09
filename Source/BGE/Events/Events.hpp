/*******************************************************************************
 * @file   Events.hpp
 * @author Brian Hoffpauir
 * @date   12.09.2024
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
#ifndef _BGE_EVENTS_HPP_
#define _BGE_EVENTS_HPP_

// TODO: Split declarations into several files by category.

namespace BGE
{
	class IEventData; // Forware declare
	BGE_DECLARE_PTR(IEventData);

	/**
	 * @brief .
	 */
	class EventData_GraphicsStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0xF0750CE9;
	public:
		EventData_GraphicsStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_GraphicsStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_PhysicsStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x4659EB7D;
	public:
		EventData_PhysicsStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_PhysicsStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_EventSystemStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x70125747;
	public:
		EventData_EventSystemStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_EventSystemStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_SoundSystemStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x76936881;
	public:
		EventData_SoundSystemStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_SoundSystemStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_LocalizationStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x9ABF60A3;
	public:
		EventData_LocalizationStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_LocalizationStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_ResourceCacheStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x72AF1E61;
	public:
		EventData_ResourceCacheStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_ResourceCacheStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_NetworkStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x3B75371B;
	public:
		EventData_NetworkStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_NetworkStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_GameLogicStarted : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0xBC9E18FA;
	public:
		EventData_GameLogicStarted(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_GameLogicStarted"; }
	};

	/**
	 * @brief .
	 */
	class EventData_GamePaused : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x490CD74B;
	public:
		EventData_GamePaused(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_GamePaused"; }
	};

	/**
	 * @brief .
	 */
	class EventData_GameResumed : public BaseEventData
	{
	public:
		static constexpr EventType kEVENT_TYPE = 0x894319F7;
	public:
		EventData_GameResumed(void) = default;
		// IEventData's interface:
		virtual const EventType &VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return "EventData_GameResumed"; }
	};
} // End namespace (BGE)

#endif /* !_BGE_EVENTS_HPP_ */
