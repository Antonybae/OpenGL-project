#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum MovementDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
const float defaultYaw = -90.0f;
const float defaultPitch = 0.0f;
const float defaultSpeed = 2.5f;
const float defaultSens = 0.1f;
const float defaultZoom = 45.0f;

class Camera
{
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUP;

	float m_yaw;
	float m_pitch;

	float m_speed;
	float m_sens;
	float m_zoom;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = defaultYaw, float pitch = defaultPitch):cameraFront(0.0f, 0.0f, -1.0f), m_speed(defaultSpeed), m_zoom(defaultZoom)
	{
		cameraPos = pos;
		worldUP = up;
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_speed(defaultSpeed), m_sens(defaultSens), m_zoom(defaultZoom)
	{
		cameraPos = glm::vec3(posX, posY, posZ);
		worldUP = glm::vec3(upX, upY, upZ);
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}


	glm::mat4 getMatrixView()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void process_keyboard_input(MovementDirection direction, float deltaTime)
	{
		if (direction == MovementDirection::UP)
		{
			cameraPos += m_speed * cameraFront * deltaTime;
		}
		if (direction == MovementDirection::DOWN)
		{
			cameraPos -= m_speed * cameraFront * deltaTime;
		}
		if (direction == MovementDirection::LEFT)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * m_speed * deltaTime;
		}
		if (direction == MovementDirection::RIGHT)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * m_speed * deltaTime;
		}
	}

	void process_mouse_input(float xOffset, float yOffset)
	{
		xOffset *= m_sens;
		yOffset *= m_sens; // Reversed since y-coordinates go from bottom to left

		m_yaw += xOffset;
		m_pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	void process_mouse_scroll(float offset)
	{
		if (m_zoom >= 1.0f && m_zoom <= 60.0f)
		{
			m_zoom -= offset;
		}
		if (m_zoom <= 1.0f)
		{
			m_zoom = 1.0f;
		}
		if (m_zoom >= 60.0f)
		{
			m_zoom = 60.0f;
		}
	}

	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		cameraFront = glm::normalize(front);

		cameraRight = glm::normalize(glm::cross(cameraFront, worldUP));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}
	
};


#endif