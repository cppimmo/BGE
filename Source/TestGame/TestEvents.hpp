#ifndef _TG_TESTEVENTS_HPP_
#define _TG_TESTEVENTS_HPP_

#include <Events/EventData.hpp>
#include <Events/Events.hpp>

namespace TestGame
{
	/**
	 * @brief .
	 */
	class EventData_Test : public BGE::BaseEventData
	{
	public:
		BGE_DEFINE_EVENTDATA(0xDEADBEEF, kExternal, "EventData_Test");
	public:
		EventData_Test(void) = default;

		virtual BGE::EventType VGetEventType(void) const override
		{
			return kEVENT_TYPE;
		}

		virtual BGE::EventSpecification VGetEventSpec(void) const override
		{
			return kEVENT_SPEC;
		}

		virtual BGE::StrongIEventDataPtr VCopy(void) const override
		{
			return nullptr; //std::make_shared<EventData_Test>(*this);
		}

		virtual void VSerialize(std::ostringstream &oss) const override
		{
		}

		virtual void VDeserialize(std::istringstream &iss) override
		{
		}

		virtual constexpr std::string_view VGetName(void) const override
		{
			return kEVENT_NAME;
		}
	};
} // End namespace (TestGame)

#endif /* !_TG_TESTEVENTS_HPP_ */
