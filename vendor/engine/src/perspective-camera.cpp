#include "engine/include/graphics/camera/perspective-camera.h"

namespace engine {

	/*
		Constructs glm orthographic projection and inserts custom position parameters
	*/
	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fov) :
		m_ProjectionMatrix(glm::perspective(fov, aspectRatio, 0.1f, 10000.0f)), 
		m_ViewMatrix(1.0f) {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		Sets the camera view projection matrix based on parameters and current view matrix
	*/
	void PerspectiveCamera::setProjection(float aspectRatio, float fov) {
		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 10000.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		This function needs to be run each time the camera is set or translated, to account for
		other objects on screen having changed their position or rotation.
	*/
	void PerspectiveCamera::resetViewMatrix() {
		glm::mat4 view = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
		
		view = glm::rotate(view, glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f));	//Rotation x-axis
		view = glm::rotate(view, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));	//Rotation y-axis
		view = glm::rotate(view, glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f));	//Rotation z-axis
		m_ViewMatrix = view;
		/*
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		*/
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}