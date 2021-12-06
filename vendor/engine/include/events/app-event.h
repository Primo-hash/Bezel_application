#pragma once

#include "event.h"

namespace engine {

	//WindowResize handler
	class WindowResizeEvent : public Event {
	public:
		//Constructor
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}
		
		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height;  }

		//Override for toString method
		std::string toString() const {
			std::stringstream ss;
			ss << "WindowResizeEvent: width:  " << m_Width << ", height: " << m_Height;
			return ss.str();
		}
		//Defining type and category function  from the enumerates in event.h
		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height; //Size of Window
	};

	//Handler for closing the Window
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent(){}
		//Defining type and category
		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}