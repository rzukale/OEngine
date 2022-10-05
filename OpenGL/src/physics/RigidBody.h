#pragma once

#include "glm/glm.hpp"

class RigidBody
{
public:

	RigidBody(float mass = 1.0f, glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Velocity = glm::vec3(0.0f), glm::vec3 Acceleration = glm::vec3(0.0f));

	float m_Mass;
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;

	void Update(float DeltaTime);

	void ApplyForce(glm::vec3 Force);
	void ApplyForce(glm::vec3 Direction, float Magnitude);

	void ApplyAcceleration(glm::vec3 Acceleration);
	void ApplyAcceleration(glm::vec3 Direction, float Magnitude);

	void ApplyImpulse(glm::vec3 Force, float DeltaTime);
	void ApplyImpulse(glm::vec3 Direction, float Magnitude, float DeltaTime);

	void TransferEnergy(float Joules, glm::vec3 Direction);
};
