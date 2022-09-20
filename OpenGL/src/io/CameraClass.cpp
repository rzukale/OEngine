#include "CameraClass.h"

CameraClass::CameraClass(glm::vec3 Position)
    : m_CameraPosition(Position), m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(0.0f), m_Pitch(0.0f),
    m_Speed(2.5f), m_Zoom(45.0f), m_CameraFront(glm::vec3(1.0f, 0.0f, 0.0f))
{
    UpdateCameraVectors();
}

void CameraClass::UpdateCameraDirection(const float& DeltaX, const float& DeltaY)
{
    m_Yaw += DeltaX;
    m_Pitch += DeltaY;

    if (m_Pitch > 89.0f)
    {
        m_Pitch = 89.0f;
    }
    else if (m_Pitch < -89.0f)
    {
        m_Pitch = -89.0f;
    }
    UpdateCameraVectors();
}

void CameraClass::UpdateCameraPosition(CameraDirection Direction, const float& DeltaTime)
{
    float Velocity = DeltaTime * m_Speed;

    switch (Direction)
    {
        case CameraDirection::FORWARD:
            m_CameraPosition += m_CameraFront * Velocity;
            break;
        case CameraDirection::BACKWARD:
            m_CameraPosition -= m_CameraFront * Velocity;
            break;
        case CameraDirection::RIGHT:
            m_CameraPosition += m_CameraRight * Velocity;
            break;
        case CameraDirection::LEFT:
            m_CameraPosition -= m_CameraRight * Velocity;
            break;
        case CameraDirection::UP:
            m_CameraPosition += m_WorldUp * Velocity;
            break;
        case CameraDirection::DOWN:
            m_CameraPosition -= m_WorldUp * Velocity;
            break;
        default:
            break;
    }
}

void CameraClass::UpdateCameraZoom(const float& DeltaY)
{
    if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
    {
        m_Zoom -= DeltaY;
    }
    else if (m_Zoom < 1.0f)
    {
        m_Zoom = 1.0f;
    }
    else
    {
        m_Zoom = 45.0f;
    }
}

float CameraClass::GetZoom()
{
    return m_Zoom;
}

glm::mat4 CameraClass::GetViewMatrix()
{
    return glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);
}

void CameraClass::UpdateCameraVectors()
{
    glm::vec3 Direction;
    Direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    Direction.y = sin(glm::radians(m_Pitch));
    Direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_CameraFront = glm::normalize(Direction);

    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
    m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
}
