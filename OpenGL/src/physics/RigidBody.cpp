#include "RigidBody.h"

RigidBody::RigidBody(float mass, glm::vec3 Position, glm::vec3 Velocity, glm::vec3 Acceleration)
	:	m_Mass(mass), m_Position(Position), m_Velocity(Velocity), m_Acceleration(Acceleration)
{
}

void RigidBody::Update(float DeltaTime)
{
	m_Position += m_Velocity * DeltaTime + 0.5f * m_Acceleration * (DeltaTime * DeltaTime);
	m_Velocity += m_Acceleration * DeltaTime;
}

void RigidBody::ApplyForce(glm::vec3 Force)
{
	m_Acceleration += Force / m_Mass;
}

void RigidBody::ApplyForce(glm::vec3 Direction, float Magnitude)
{
	ApplyForce(Direction * Magnitude);
}

void RigidBody::ApplyAcceleration(glm::vec3 Acceleration)
{
	m_Acceleration += Acceleration;
}

void RigidBody::ApplyAcceleration(glm::vec3 Direction, float Magnitude)
{
	ApplyAcceleration(Direction * Magnitude);
}

void RigidBody::ApplyImpulse(glm::vec3 Force, float DeltaTime)
{
	m_Velocity += Force / m_Mass * DeltaTime;
}

void RigidBody::ApplyImpulse(glm::vec3 Direction, float Magnitude, float DeltaTime)
{
	ApplyImpulse(Direction * Magnitude, DeltaTime);
}

void RigidBody::TransferEnergy(float Joules, glm::vec3 Direction)
{
	if (Joules == 0)
	{
		return;
	}
	glm::vec3 DeltaV = sqrt(2 * abs(Joules) / m_Mass) * Direction;

	m_Velocity += Joules > 0 ? DeltaV : -DeltaV;
}

