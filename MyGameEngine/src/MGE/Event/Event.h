#pragma once

#include "EventTypes.h"

#include <string>
#include <functional>
#include <iostream>

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return Eventype::##type;} \
																virtual EventType GetEventType() const override { return GetStaticType(); } \
																virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; } 

namespace MGE {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	class MGE_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() = 0;
		virtual std::string ToString() const{ return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		// Alias for a function that takes an event of type T and returns a boolean indicating whether the event was handled or not.
		template<typename EventT>
		using EventHandlerFunction = std::function<bool(EventT&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// Dispatch function that takes an event handler function as its argument. 
		// If the event type matches the type of the event handler function, it calls the function with the event and returns true. 
		// If the types don't match, it simply returns false.

		template<typename EventT>
		bool Dispatch(EventHandlerFunction <EventT> handlerFunction) {
			if (m_Event.GetEventType() == EventT::GetStaticType()) {
				m_Event.m_handled = handlerFunction(*(EventT*)&e_Event);
				return true;
			}
			return false;
		}

	protected:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}