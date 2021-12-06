#pragma once
#include "engine/precompiled.h"
#include "engine/include/core.h"
#include "orthographic-camera.h"
#include "perspective-camera.h"
#include "engine/include/time.h"

#include "engine/include/events/app-event.h"
#include "engine/include/events/mouse-event.h"

#include "engine/include/input.h"

namespace engine {

	/*
		Handles PerspectiveCamera objects
	*/
	class CameraController {
	public:
		CameraController(float aspectRatio, float fov = 90.f, bool rotation = true, bool flight = true);

		void onUpdate(Time ts);
		void onEvent(Event& e);

		PerspectiveCamera& getCamera() { return m_Camera; }
		const PerspectiveCamera& getCamera() const { return m_Camera; } // Incase a const camera obj is needed

		float getZoomLevel() const { return m_ZoomLevel; }
		void setZoomLevel(float level);

	private:
		float m_AspectRatio;								// Aspect ratio to match the window
		float m_ZoomLevel = 1.0f;							// Camera scaling
		float m_FOV = 90.f;									// Field of view
		float m_Yaw = -90.0f;									// Initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
		float m_Pitch = 0.0f;
		PerspectiveCamera m_Camera;

		bool m_Rotation;									// Camera rotation capability switch
		bool m_Flight;										// Camera ascension and decension capability switch 
		bool m_FirstMousePosition = true;					// For setting initial looking direction position

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, -10.0f };	// Initial camera position
		glm::vec3 m_LastMousePosition = { 0.0f, 0.0f, 0.0f };	// Last mouse position for handling of looking direction
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };		// Per axis
		float m_LookSensitivity = 0.7f;							// Looking sensitivity when using mouse
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

		bool onWindowResized(WindowResizeEvent& e);			// Event handling of camera on window resize events
	};

	/*
		Handles OrthographicCamera objects
	*/
	class OrthoCameraController {
	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Time ts);
		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return m_Camera; }
		const OrthographicCamera& getCamera() const { return m_Camera; } // Incase a const camera obj is needed

		float getZoomLevel() const { return m_ZoomLevel; }
		void setZoomLevel(float level);

	private:
		float m_AspectRatio;								// Aspect ratio to match the window
		float m_ZoomLevel = 1.0f;							// Camera scaling
		OrthographicCamera m_Camera;

		bool m_Rotation;									// Camera rotation on/off not part of Assig1

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	// Initial camera position
		// Below private class members and event functions not in use in Assig1
		float m_CameraRotation = 0.0f;						// In degrees
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

		bool onWindowResized(WindowResizeEvent& e);			// Event handling of camera on window resize events
	};

}