#pragma once

#include "event.h"

namespace engine {

	/*
	 Class KeyEvent is a subclass of Event and abstracts different events of Key actions in the application
	*/
	class KeyEvent : public Event {
	public:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}
		// getKeyCode returns the codeNumber of the defined event key
		inline int getKeyCode() const { return m_KeyCode; }
		// Define Event Category as both Keyboard and Input
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		int m_KeyCode;
	};

	/*
		KeyPressedEvent used to check when a key is pressed and log the amount of repetitions.

		An example use case for getRepeatCount would be to check when a key has
		been repeated 10 times for an interaction to happen in game.
	*/
	class KeyPressedEvent : public KeyEvent {
	public:
		//Constructor
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		//Returns number of times the defined event key has been repeated
		inline int getRepeatCount() const { return m_RepeatCount; }

		//Override ToString
		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		//Defining Event Type
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		//Number of times key has been pressed
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}