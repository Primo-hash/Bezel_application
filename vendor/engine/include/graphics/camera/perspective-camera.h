#pragma once

#include "engine/precompiled.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

	/*
		The OrthographicCamera class acts as a device to capture a 2D view projection
		Its members are factored in with a shader program to achieve this effect when the
		renderer issues a draw call.
	*/
	class PerspectiveCamera {
	public:
		PerspectiveCamera(float aspectRatio, float fov = 90.f);

		void setProjection(float aspectRatio, float fov = 90.f);

		const glm::vec3& getPosition() const { return m_Position; }
		void setPosition(const glm::vec3& position) { m_Position = position; resetViewMatrix(); }

		glm::vec3 getRotation() const { return m_Rotation; }
		void setRotation(glm::vec3 rotation) { m_Rotation = rotation; resetViewMatrix(); }

		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const glm::vec3& getFront() const { return m_CameraFront; }
		void setFront(glm::vec3 front) { m_CameraFront = front; }

		const glm::vec3& getUp() const { return m_CameraUp; }
		const glm::vec3& getRight() const { return m_CameraRight; }
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// Camera orientation
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);	// Direction of camera view
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		// Which axis is up direction
		glm::vec3 m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);	// Which axis is down direction
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };			// Position in space
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };			// Rotation in space

		void resetViewMatrix();
	};
}