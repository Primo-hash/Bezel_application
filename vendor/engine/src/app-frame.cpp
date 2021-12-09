#include "engine/include/app-frame.h"

namespace engine {

	AppFrame* AppFrame::s_Instance = nullptr;

	AppFrame::AppFrame() {
		// This static instance of the class itself allows us to call a single
		// shared instance of the Application frame between classes without defining or
		// referencing a new application frame object multiple times since we're only using one anyways.
		// Very helpful and saves space for function parameters
		ENGINE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Simple test window to check the Window class and children's functionality
		m_Window = std::unique_ptr<Window>(Window::create());
		// Default set of keyboard, mouse and application events running by default
		m_Window->setEventCallback(BIND_EVENT_FN(AppFrame::onEvent));
	}

	/*
		Constructor that takes custom window data
	*/
	AppFrame::AppFrame(WindowSpecs specs) : m_WindowSpecs(specs) {
		// This static instance of the class itself allows us to call a single
		// shared instance of the Application frame between classes without defining or
		// referencing a new application frame object multiple times since we're only using one anyways.
		// Very helpful and saves space for function parameters
		ENGINE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Simple Window class w/ children's functionality
		m_Window = std::unique_ptr<Window>(Window::create(m_WindowSpecs));
		// Default set of keyboard, mouse and application events running by default
		m_Window->setEventCallback(BIND_EVENT_FN(AppFrame::onEvent));
	}

	AppFrame::~AppFrame() {

	}

	/*
	Close window protocol
*/
	void AppFrame::closeWindow() {
		m_Running = false;
	}

	/*
		Mandatory functions to run for applications
	*/
	void AppFrame::run() {
		while (m_Running) {	// Application loop

			// Time
			float time = (float)glfwGetTime();
			Time timecycle = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Handle events bottom of stack has priority
			// Stops iteration if event has been handled
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it) {
				(*it)->onUpdate(timecycle);
			}

			m_Window->onUpdate();
		}
	}

	/*
		Returns signal to close the referenced application window
		and sets values to represent shutdown.
	*/
	bool AppFrame::onWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	/*
		Uses event dispatcher to handle execution and logging of
		any event function type and instance of this class.
	*/
	void AppFrame::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		// Checks if window closing event has been called
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(AppFrame::onWindowClose));

		// Handle events in reverse top of stack has priority
		// Stops iteration if event has been handled
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			(*it)->onEvent(e);
			if (e.m_Handled) { break; }
		}
	}

	/*
		Sets the current application's window icon to the specified image path
	*/
	void AppFrame::setAppIcon(std::string path) {
		auto window = static_cast<GLFWwindow*>(m_Window->getNativeWindow());
		GLFWimage windowIcon[1];

		windowIcon[0].pixels = stbi_load(path.c_str(), &windowIcon[0].width, &windowIcon[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(window, 1, windowIcon);
		stbi_image_free(windowIcon[0].pixels);
	}

	void AppFrame::pushLayer(Layer* layer) {
		// Insert Layer at the beginning of the vector
		// Allocate more space if needed
		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->onAttach();				// Run construction protocol
	}
	
	/*
		Probably won't need to pop a layer for pacman but good to have
	*/
	void AppFrame::popLayer(Layer* layer) {
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.begin() + m_LayerInsertIndex, layer);
		if (it != m_LayerStack.begin() + m_LayerInsertIndex) {
			layer->onDetach();			// Run destruction protocol
			m_LayerStack.erase(it);
			m_LayerInsertIndex--;		// Moves starting pointer one element backward
		}
	}
}