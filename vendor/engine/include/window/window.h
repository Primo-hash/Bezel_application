/*
	window.h handles the GLFW functions required to setup and 
	handle a window instance for OpenGL applications.
*/
#pragma once
#include "engine/include/core.h"
#include "engine/include/events/event.h"
#include "engine/precompiled.h"
#include "engine/include/window/window-context.h"


#include "GLFW/glfw3.h"			// OpenGL Window

namespace engine {

	// Define a callback function for window events
	using EventCallbackFn = std::function<void(Event&)>;

	/*
		Stores specified window parameters
	*/
	struct WindowSpecs
	{
		std::string title;
		unsigned int width, height;
		bool VSync;

		EventCallbackFn eventCallBack;

		// Default values for window specifications unless specified otherwise in constructor
		WindowSpecs(const std::string& title = "Engine Window", unsigned int width = 1080, unsigned int height = 720) : 
			title(title), 
			width(width), 
			height(height) {}
	};

	class Window {
	public:
		Window(const WindowSpecs& specs);
		virtual ~Window();

		// Changes to be made/checked before the next render cycle
		void onUpdate();

		inline unsigned int getWidth() const { return m_Specs.width; }
		inline unsigned int getHeight() const { return m_Specs.height; }

		inline void closeWindow() { shutdown(); };

		// Attributes
		inline void setEventCallback(const EventCallbackFn& callback) { m_Specs.eventCallBack = callback; }
		void setVSync(bool enabled);
		bool isVSync() const;

		// To get native window types like GLFWwindow from GLFW for example.
		inline virtual void* getNativeWindow() const { return m_Window; }

		// Function for serving window instances
		static u_Ptr<Window> create(const WindowSpecs& specs = WindowSpecs());
	private:
		GLFWwindow* m_Window;
		WindowSpecs m_Specs;
		u_Ptr<WindowContext> m_Context;

		virtual void shutdown();
	};
}