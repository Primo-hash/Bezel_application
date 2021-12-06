#include "engine/include/window/window-context.h"

// Engine tools
#include "engine/include/core.h"
#include "engine/include/logger.h"
#include "engine/precompiled.h"

namespace engine {

	/*
		Defines window to use for context switching and changes context to that window.
		Logs current OpenGL details and graphics card details
	*/
	WindowContext::WindowContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		ENGINE_ASSERT(windowHandle, "Window handle is null!")	// Error check window

		glfwMakeContextCurrent(m_WindowHandle);		// Set context to new window
		// GLFW uses glad in library as well and sometimes their definitions and declarations
		// override eachother, this is to check if that happens because of changes to the engine
		// An example would be the include order of GLFW and glad in the file
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	
		ENGINE_ASSERT(status, "Failed to initialize Glad!");

		// Log OpenGL/Graphics details
		ENGINE_INFO("OpenGL Info:");
		ENGINE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
		ENGINE_INFO("\tRenderer: {0}", glGetString(GL_RENDERER));
		ENGINE_INFO("\tVersion: {0}", glGetString(GL_VERSION));
	}

	/*
		Bufferswap between displayed draw and current draw to mitigate problems like tearing
	*/
	void WindowContext::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}