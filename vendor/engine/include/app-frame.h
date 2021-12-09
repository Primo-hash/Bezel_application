#pragma once
#include "engine/precompiled.h"
#include "core.h"
#include "logger.h"

#include "events/event.h"
#include "events/app-event.h"
#include "events/key-event.h"
#include "events/mouse-event.h"

#include "window/window.h"
#include "layer.h"
#include "time.h"
#include "input.h"

#include "engine/vendor/stb/src/stb_image.h"

namespace engine {

	/*
		AppFrame is an interface class for all applications that want access to this engine's tools
	*/
	class AppFrame {
	public:
		AppFrame();
		AppFrame(WindowSpecs specs);
		virtual ~AppFrame();		// Inherited as an interface by applications using the engine lib

		void run();					// Application runtime functions
		void onEvent(Event& e);		// Handles events with dispatcher
	
		// Close window instance
		void closeWindow();

		// Returns pointer to application window
		inline Window& getWindow() { return *m_Window; }

		// Returns pointer to application instance
		inline static AppFrame& get() { return *s_Instance; }

		void setAppIcon(std::string path);		// Creates an application icon 

		void pushLayer(Layer* layer);			// Inserts layer to LayerStack
		void popLayer(Layer* layer);			// Pops a layer from the LayerStack
	private:
		static AppFrame* s_Instance;			// Application instance called by client/engine
		bool m_Running = true;

		// Time
		float m_LastFrameTime;

		// Window
		u_Ptr<Window> m_Window;
		WindowSpecs m_WindowSpecs;

		// Layer handling
		std::vector<Layer*> m_LayerStack;	// Vector treated as stack
		unsigned int m_LayerInsertIndex = 0;

		bool onWindowClose(WindowCloseEvent& e);
	};

	u_Ptr<AppFrame> createApp();		// Initialized here, and defined in client application
}