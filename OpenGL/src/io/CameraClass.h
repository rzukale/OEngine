#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class CameraClass
{
public:
	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;
	glm::vec3 m_CameraRight;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;
	float m_Speed;
	float m_Zoom;

	CameraClass(glm::vec3 Position);

	void UpdateCameraDirection(const float& DeltaX, const float& DeltaY);
	void UpdateCameraPosition(CameraDirection Direction, const float& DeltaTime);
	void UpdateCameraZoom(const float& DeltaY);

	float GetZoom();

	glm::mat4 GetViewMatrix();

private:
	void UpdateCameraVectors();
};

