#pragma once
#include "engine/precompiled.h"
#include "core.h"
#include "engine/include/app-frame.h"
#include <GLFW/glfw3.h>

namespace engine {

	class Input {
	public:
		// Custom implmentations of input protocol
		Input() = default;

		inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedCustom(keycode); }
		inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedCustom(button); }
		inline static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionCustom(); }
		inline static float getMouseX() { return s_Instance->getMouseXCustom(); }
		inline static float getMouseY() { return s_Instance->getMouseYCustom(); }
	
	private:
		static u_Ptr<Input> s_Instance;		// Custom input instance for desktop

		virtual bool isKeyPressedCustom(int keycode);
		virtual bool isMouseButtonPressedCustom(int button);
		virtual std::pair<float, float> getMousePositionCustom();
		virtual float getMouseXCustom();
		virtual float getMouseYCustom();
	};
}