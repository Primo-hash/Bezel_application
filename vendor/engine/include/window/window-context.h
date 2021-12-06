#pragma once
// OpenGL library headers
#include <GLFW/glfw3.h>		// Window
#include <glad/glad.h>		// OpenGL func
#include <GL/gl.h>			// keep gl.h lowercase for linux

// GLFW native window needs to be declared to use
struct GLFWwindow;

namespace engine {

	/*
		OpenGL specific implementation class for context switching a native GLFW window
	*/
	class WindowContext {
	public:
		WindowContext(GLFWwindow* windowHandle);
		virtual void swapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};

}