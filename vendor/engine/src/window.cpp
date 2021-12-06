#include "engine/include/logger.h" 
#include "engine/include/window/window.h"

// Window events for callback functions
#include "engine/include/events/app-event.h"
#include "engine/include/events/mouse-event.h"
#include "engine/include/events/key-event.h"

namespace engine {

	static bool s_GLFWInitialized = false;

	// FUNCTIONS

	/*
		Error handler for GLFW function callbacks, prints error and description.
	*/
	static void GLFWErrorCallback(int error, const char* desc) {
		ENGINE_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	// CLASS FUNCTIONS

	/*
		Returns pointer to new Window object initialized with WindowSpecs object parameter
		this is an alternative to creating window instances without constructing this class.
		Can be used to setup a window before construction.
	*/
	u_Ptr<Window> Window::create(const WindowSpecs& specs) {
		return m_UPtr<Window>(specs);
	}

	/*
		Sets up window data on window object creation
		initializes desktop window with specified data by user.
		Logs window specs while creating.
	*/
	Window::Window(const WindowSpecs& specs) {
		m_Specs.title = specs.title;
		m_Specs.width = specs.width;
		m_Specs.height = specs.height;

		ENGINE_INFO("Creating window {0} ({1}, {2})", specs.title, specs.width, specs.height);

		if (!s_GLFWInitialized) {	// GLFW initialization with error logging
			int success = glfwInit();
			ENGINE_ASSERT(success, "Could not intialize GLFW!");

			// Ensures GLFW Errors are printed with the logging system
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// Create native GLFW window
		m_Window = glfwCreateWindow((int)specs.width, (int)specs.height, m_Specs.title.c_str(), nullptr, nullptr);

		// Set OpenGL to render this window context (m_Window)
		m_Context = m_UPtr<WindowContext>(m_Window);

		// Context options
		glfwSetWindowUserPointer(m_Window, &m_Specs);	// Window context details
		setVSync(false);									// Vsync off by default

		/*
		The following callback functions utilize the GLFW library to abstract window events
		while tying them to a certain application's window object (possibly multiple).
		This allows us to customize certain events for the window like logging events while executing them
		since the GLFW functions allow the passing of certain functions to be executed after the desired event.

		These functions are made using lambdas and are tied to each Window object on construction respectively
		This allows us to create Windows with different Events implemented based on different functions they were constructed with.
		*/

		/*
			Callback function glfwSetWindowSizeCallback for logging the resizing of the window.
		*/
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);	// Shorten GLFW window name pointer to data.
			specs.width = width;
			specs.height = height;

			WindowResizeEvent event(width, height);
			specs.eventCallBack(event);
		});

		/*
			Callback function glfwSetWindowCloseCallback for logging the termination of the window.
		*/
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			specs.eventCallBack(event);
		});

		/*
			Callback function glfwSetKeyCallback for the use of keys within the window.
		*/
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);

			// We have made events for each of these key actions allowing better control of them.
			// An example would be the repeat event being executed a limited number of times instead of continuing.
			// This allows for better limiting or increasing player controls.
			switch (action) {
			case GLFW_PRESS:
			{
				switch (key) {
				case GLFW_KEY_ESCAPE: {				// Window closes on keypress escape
						WindowCloseEvent event;		// Calls window closing event
						specs.eventCallBack(event);
					}
				}

				KeyPressedEvent event(key, 0);
				specs.eventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				specs.eventCallBack(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				specs.eventCallBack(event);
				break;
			}
			}
		});

		/*
			Callback function glfwSetCharCallback for registering key typing (e.g. in a textbox) and logging the event.
		*/
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			specs.eventCallBack(event);
		});

		/*
			For handling and logging mouse click events
		*/
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				specs.eventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				specs.eventCallBack(event);
				break;
			}
			}
		});

		/*
			For handling and logging mouse movement events
		*/
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowSpecs& specs = *(WindowSpecs*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			specs.eventCallBack(event);
		});
}

	Window::~Window() {
		shutdown();
	}

	// The following functions are basic window actions to be utilized 
	// by callback functions or the like.

	void Window::shutdown() {
		glfwDestroyWindow(m_Window);
		ENGINE_INFO("Terminating GLFW");
		glfwTerminate();
	}

	void Window::onUpdate() {
		/*
			This function probes for any changes done each game loop cycle.
			This could be for example the events that are defined in the glfw callback lambdas
			defined in the class constructor.
		*/
		glfwPollEvents();
		m_Context->swapBuffers();
	}

	void Window::setVSync(bool enabled) {
		if (enabled)	// Set vsync on or off based on GLFW lib functions
			glfwSwapInterval(1);	// On
		else
			glfwSwapInterval(0);	// Off

		m_Specs.VSync = enabled;	// Set vsync indicator based on param
	}

	bool Window::isVSync() const {
		return m_Specs.VSync;	// Check if vsync on
	}	

}