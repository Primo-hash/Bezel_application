#include "engine/include/graphics/camera/orthographic-camera.h"

namespace engine {

	/*
		Constructs glm orthographic projection and inserts custom position parameters
	*/
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 100.0f)), m_ViewMatrix(1.0f) {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		Sets the camera view projection matrix based on parameters and current view matrix
	*/
	void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 100.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/*
		This function needs to be run each time the camera is set or translated, to account for
		other objects on screen having changed their position or rotation.
	*/
	void OrthographicCamera::resetViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}