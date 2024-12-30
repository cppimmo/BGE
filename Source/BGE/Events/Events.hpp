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
		BGE_DEFINE_EVENTDATA(0xF0750CE9, kInternal, "EventData_GraphicsStarted");
	public:
		EventData_GraphicsStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_PhysicsStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x4659EB7D, kInternal, "EventData_PhysicsStarted");
	public:
		EventData_PhysicsStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_EventSystemStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x70125747, kInternal, "EventData_EventSystemStarted");
	public:
		EventData_EventSystemStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_SoundSystemStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x76936881, kInternal, "EventData_SoundSystemStarted");
	public:
		EventData_SoundSystemStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_LocalizationStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x9ABF60A3, kInternal, "EventData_LocalizationStarted");
	public:
		EventData_LocalizationStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_ResourceCacheStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x72AF1E61, kInternal, "EventData_ResourceCacheStarted");
	public:
		EventData_ResourceCacheStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_ScriptingSystemStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x4998957A, kInternal, "EventData_ScriptingSystemStarted");
	public:
		EventData_ScriptingSystemStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_NetworkStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x3B75371B, kInternal, "EventData_NetworkStarted");
	public:
		EventData_NetworkStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_GameLogicStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0xBC9E18FA, kInternal, "EventData_GameLogicStarted");
	public:
		EventData_GameLogicStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_DebugConsoleStarted : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x01D9FFAB, kInternal, "EventData_DebugConsoleStarted");
	public:
		EventData_DebugConsoleStarted(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_GamePaused : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x490CD74B, kInternal, "EventData_GamePaused");
	public:
		EventData_GamePaused(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};

	/**
	 * @brief .
	 */
	class EventData_GameResumed : public BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0x894319F7, kInternal, "EventData_GameResumed");
	public:
		EventData_GameResumed(void) = default;
		// IEventData's interface:
		virtual EventType VGetEventType(void) const override { return kEVENT_TYPE; }
		virtual EventSpecification VGetEventSpec(void) const override { return kEVENT_SPEC; }
		virtual StrongIEventDataPtr VCopy(void) const override { return nullptr; }
		virtual void VSerialize(std::ostringstream &oss) const override { }
		virtual void VDeserialize(std::istringstream &iss) override { }
		virtual constexpr std::string_view VGetName(void) const override { return kEVENT_NAME; }
	};
} // End namespace (BGE)

#endif /* !_BGE_EVENTS_HPP_ */
