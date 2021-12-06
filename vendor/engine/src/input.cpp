/*
	Implementation of input polling for desktop platforms.
	Allows uses like asking if certain keys are being pressed to GLFW.

	Using GLFW so both UNIX and WINDOWS use the same functions

	This class expands GLFW key codes and needs them to operate.
	For example calling a function may look like: isKeyPressed(GLFW_KEY_SPACE)
	All GLFW key codes are referenced in glfw3.h from line 392 an below.
	*/
#include "engine/include/input.h"

namespace engine {

	u_Ptr<Input> Input::s_Instance = m_UPtr<Input>();	// Input instance init itself

	/*
		Custom key press function utlizing GLFW
	*/
	bool Input::isKeyPressedCustom(int keycode) {
		auto window = static_cast<GLFWwindow*>(AppFrame::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	/*
		Custom mouse button press function utlizing GLFW
	*/
	bool Input::isMouseButtonPressedCustom(int button) {
		auto window = static_cast<GLFWwindow*>(AppFrame::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	/*
		Custom mouse position return function utlizing GLFW
	*/
	std::pair<float, float> Input::getMousePositionCustom() {
		auto window = static_cast<GLFWwindow*>(AppFrame::get().getWindow().getNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };

	}

	/*
		Custom mouse X position return function utlizing GLFW
	*/
	float Input::getMouseXCustom() {
		auto [x, y] = getMousePositionCustom();
		return x;

	}

	/*
		Custom mouse Y position return function utlizing GLFW
	*/
	float Input::getMouseYCustom() {
		auto [x, y] = getMousePositionCustom();
		return y;

	}
}
