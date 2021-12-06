#pragma once
#include "engine/precompiled.h"
#include "engine/include/core.h"

namespace engine {

	//List of different event types based on
	//relevant event instance
	enum class EventType 
	{
		WindowClose, WindowResize,
		MouseButtonPressed, MouseButtonReleased, MouseMoved,
		KeyPressed, KeyReleased, KeyTyped
	};

	#pragma warning(push)	// Removes a warning that is a best practices suggestion which doesn't apply on this project.
	#pragma warning(disable : 26812)
	/*
		Definert event kategorier for filtrering av kategorier gitt tillatelse basert på instans
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication,
		EventCategoryInput,
		EventCategoryKeyboard,
		EventCategoryMouse,
		EventCategoryMouseButton
	};
	#pragma warning(pop)
/*
  EventType and Categories for macro 
  These macros are made to avoid repeating the same mandatory definition
  of virtual functions in the Event subclasses. basically we shorthen the 4 lines of code below
  down to 2 for each subclass.

  utilizes override functions to use subclass definition instead of superclass one
*/
#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
		virtual EventType getEventType() const override { return getStaticType(); }\
		virtual const char* getName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	//Baseclass of Event 
	class Event {
		friend class EventDispatcher;

	public:
		/*
			We need a flag to check if an event has been within a certain scope (a layer for example)
			such that the event does not propagate and downwards to different scopes.

			One such example would be that selecting a menu option using arrow keys while paused ingame does not
			move the character as well.

			this is why the m_handled is used to check for that instance
		*/
		bool m_Handled = false;

		//Virtual functions to be overriden by inheriting subclasses
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		//Returns true if category is in defined categories
		inline bool isInCategory(EventCategory category) { return getCategoryFlags() &category; }
	};

	/*
	  EventDispatcher handles the setup and dispatching of events

	  Takes an event as a reference for construction.
	  Later on we can check if this event is still happening by calling
	  the dispatch function with the current event.
	*/
	class EventDispatcher {
	private:
		Event& m_Event;	// Event to be referenced for dispatch
	public:
		//Constructor - recieve any events as a reference
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			//Check if member event type is equal to class type
			if (m_Event.getEventType() == T::getStaticType()) { // If the intrinsic event matches the custom event function
				// We can run that custom function to achieve desired results.
				// An example would be setting up a window closing event and
				// sending a lambda as a parameter where we could free some memory.
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				// Allows us to create a custom event function that returns a boolean
				return true;
			}
			else {
				return false;
			}
		}

		/*
			Adds functionality to output stream operator for logging of custom events by string.
		*/
		friend inline std::ostream& operator << (std::ostream& os, const Event& e) {
			return os << e.toString();
		}
	};
}