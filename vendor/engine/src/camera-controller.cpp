#include "engine/include/graphics/camera/camera-controller.h"

namespace engine {

	/*
		On construction camera is set to fit the aspect ratio value provided by the application.
	*/
	CameraController::CameraController(float aspectRatio, float fov, bool rotation, bool flight) :
		m_AspectRatio(aspectRatio),
		m_FOV(fov),
		// Camera view set based on aspec ratio and zoom level
		m_Camera(aspectRatio, fov),
		m_Rotation(rotation),
		m_Flight(flight) {}

	/*
		Camera movement controlled by key input events.
	*/
	void CameraController::onUpdate(Time ts) {

		// Camera movement controls
		if (Input::isKeyPressed(GLFW_KEY_A)) {
			// Utilize crossproduct to get side movement based on looking direction
			m_CameraPosition -= glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * m_CameraTranslationSpeed * ts.getSeconds();
		}
		else if (Input::isKeyPressed(GLFW_KEY_D)) {
			m_CameraPosition += glm::normalize(glm::cross(m_Camera.getFront(), m_Camera.getUp())) * m_CameraTranslationSpeed * ts.getSeconds();
		}
		if (Input::isKeyPressed(GLFW_KEY_W)) {
			m_CameraPosition += m_Camera.getFront() * m_CameraTranslationSpeed * ts.getSeconds();
		}
		else if (Input::isKeyPressed(GLFW_KEY_S)) {
			m_CameraPosition -= m_Camera.getFront() * m_CameraTranslationSpeed * ts.getSeconds();
		}

		// Camera flight controls
		if (m_Flight) {
			
			if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
				m_CameraPosition += glm::normalize(m_Camera.getUp() * m_CameraTranslationSpeed * ts.getSeconds());
			}
			else if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				m_CameraPosition -= glm::normalize(m_Camera.getUp() * m_CameraTranslationSpeed * ts.getSeconds());
			}
			
		}

		// Camera Rotation if set on construction
		if (m_Rotation) {

			
			if (Input::isKeyPressed(GLFW_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			if (Input::isKeyPressed(GLFW_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.setRotation(m_CameraRotation);
		}

		// Looking direction
		float mousePosX = Input::getMouseX();
		float mousePosY = Input::getMouseY();

		if (m_FirstMousePosition)	// Get initial position of mouse on first run
		{
			m_LastMousePosition.x = mousePosX;
			m_LastMousePosition.y = mousePosY;
			m_FirstMousePosition = false;
		}
		
		float offsetX = mousePosX - m_LastMousePosition.x;
		float offsetY = m_LastMousePosition.y - mousePosY; // reversed since y-coordinates goes from bottom to top
		m_LastMousePosition.x = mousePosX;
		m_LastMousePosition.y = mousePosY;

		offsetX *= m_LookSensitivity;	// Apply looking sensitivity
		offsetY *= m_LookSensitivity;

		m_Yaw += offsetX;
		m_Pitch += offsetY;

		// Ensure screen does not lip when pitch goes out of bounds
		if (m_Pitch > 89.0f) { m_Pitch = 89.0f; }
		if (m_Pitch < -89.0f) { m_Pitch = -89.0f; }

		glm::vec3 lookingDirection;
		lookingDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		lookingDirection.y = sin(glm::radians(m_Pitch));
		lookingDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Camera.setFront(glm::normalize(lookingDirection));

		// Update position
		m_Camera.setPosition(m_CameraPosition);
		// This line basically sets movement speed for the camera to be slow
		// when camera is zoomed in and faster when zoomed out.
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	/*
		When referenced starts an event dispatcher to bind input events to the current object.
	*/
	void CameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthoCameraController::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraController::onWindowResized));
	}

	/*
		Set a new zoom level for camera and update view projection
	*/
	void CameraController::setZoomLevel(float level) {
		m_ZoomLevel = level;
		//m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	/*
		Handles updating the projection on window resize events when referenced.
	*/
	bool CameraController::onWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();		// Get new specs from the event
		// Camera view updated based on aspec ratio and zoom level
		m_Camera.setProjection(m_AspectRatio, m_FOV);
		return false;	// In case function run with assertion, this event is important so returns signal to log
	}



	/*
		Ortho Camera controller function defintions
	*/

	/*
		On construction camera is set to fit the aspect ratio value provided by the application.
	*/
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio), 
		// Camera view set based on aspec ratio and zoom level
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), 
		m_Rotation(rotation) {}

	/*
		Camera movement controlled by key input events.
		NOT FOR ASSIG1
	*/
	void OrthoCameraController::onUpdate(Time ts) {

		// Camera Translation
		if (Input::isKeyPressed(GLFW_KEY_A)) {
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(GLFW_KEY_D)) {
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		if (Input::isKeyPressed(GLFW_KEY_W)) {
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(GLFW_KEY_S)) {
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}

		// Camera Rotation if set on construction
		if (m_Rotation) {

			if (Input::isKeyPressed(GLFW_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			if (Input::isKeyPressed(GLFW_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			if (m_CameraRotation > 180.0f) {
				m_CameraRotation -= 360.0f;
			}
			else if (m_CameraRotation <= -180.0f) {
				m_CameraRotation += 360.0f;
			}

			m_Camera.setRotation(m_CameraRotation);
		}
		m_Camera.setPosition(m_CameraPosition);		
		// This line basically sets movement speed for the camera to be slow
		// when camera is zoomed in and faster when zoomed out.
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	/*
		When referenced starts an event dispatcher to bind input events to the current object.
	*/
	void OrthoCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthoCameraController::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthoCameraController::onWindowResized));
	}

	/*
		Set a new zoom level for camera and update view projection
	*/
	void OrthoCameraController::setZoomLevel(float level) {
		m_ZoomLevel = level;
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	/*
		Handles updating the projection on window resize events when referenced.
	*/
	bool OrthoCameraController::onWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();		// Get new specs from the event
		// Camera view updated based on aspec ratio and zoom level
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;	// In case function run with assertion, this event is important so returns signal to log
	}
}
