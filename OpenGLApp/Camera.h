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
const float defaultZoom = 75.0f;

class Camera
{
public:
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

	//
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = defaultYaw, float pitch = defaultPitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_speed(defaultSpeed), m_sens(defaultSens), m_zoom(defaultZoom)
	{
		cameraPos = position;
		worldUP = up;
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}
	//
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_speed(defaultSpeed), m_sens(defaultSens), m_zoom(defaultZoom)
	{
		cameraPos = glm::vec3(posX, posY, posZ);
		worldUP = glm::vec3(upX, upY, upZ);
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}

	//
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	//
	void process_keyboard_input(MovementDirection direction, float deltaTime)
	{
		float velocity = m_speed * deltaTime;
		if (direction == UP)
		{
			cameraPos += cameraFront * velocity;
		}
		if (direction == DOWN)
		{
			cameraPos -= cameraFront * velocity;
		}
		if (direction == LEFT)
		{
			cameraPos -= cameraRight * velocity;
		}
		if (direction == RIGHT)
		{
			cameraPos += cameraRight *velocity;
		}
	}
	//
	void process_mouse_input(float xOffset, float yOffset)
	{
		xOffset *= m_sens;
		yOffset *= m_sens;

		m_yaw += xOffset;
		m_pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		
		if (m_yaw > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_yaw = -89.0f;

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}
	//
	void process_mouse_scroll(float yoffset)
	{
		if (m_zoom >= 1.0f && m_zoom <= 75.0f)
		{
			m_zoom -= yoffset;
		}
		if (m_zoom <= 1.0f)
		{
			m_zoom = 1.0f;
		}
		if (m_zoom >= 75.0f)
		{
			m_zoom = 75.0f;
		}
	}
	//
private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		cameraFront = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		cameraRight = glm::normalize(glm::cross(cameraFront, worldUP));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}
	
};
#endif