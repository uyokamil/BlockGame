//
// Created by physi on 15/12/2024.
//

#include "Camera.h"

#include <format>

#include "../Application/Application.h"

namespace BlockGame {

    Camera::Camera(float fov, float nearPlane, float farPlane, float sensitivity, glm::vec2 windowSize)
        : m_FOV(fov), m_NearPlane(nearPlane), m_FarPlane(farPlane),
          m_Sensitivity(sensitivity), m_FirstMouse(true), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
          m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
          m_Yaw(-90.0f), m_Pitch(0.0f), m_LastX(0.0f), m_LastY(0.0f),
          m_WindowSize(windowSize)
    {
        m_Aspect = m_WindowSize.x / m_WindowSize.y;
    }

    Camera::~Camera() = default;

    void Camera::Update(GLFWwindow *window, float deltaTime)
    {
        // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        //     speed = 10;
        // else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        //     speed = 5;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        HandleMouseInput(xpos, ypos);

        // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        // {
        //
        // }
        // else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        // {
        //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //     m_FirstMouse = true;
        // }

        UpdateCameraVectors();

        // TODO:
        std::string direction = GetCardinalDirection();
        std::string location = GetWorldLocationString();
        glfwSetWindowTitle(window, (("Facing: " + direction) + " | " + location).c_str());
    }

    void Camera::HandleMouseInput(double xPos, double yPos)
    {
        if (m_FirstMouse) {
            m_LastX = xPos;
            m_LastY = yPos;
            m_FirstMouse = false;
        }

        float xOffset = static_cast<float>(xPos - m_LastX) * m_Sensitivity;
        float yOffset = static_cast<float>(m_LastY - yPos) * m_Sensitivity;

        m_LastX = xPos;
        m_LastY = yPos;

        m_Yaw += xOffset;
        m_Pitch += yOffset;

        m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

        UpdateCameraVectors();
    }

    void Camera::UpdateAspectRatio(float width, float height)
    {
        if (height == 0.0f) height = 1.0f;  // Avoid division by zero
        m_WindowSize = glm::vec2(width, height);
        m_Aspect = width / height;
    }

    void Camera::SetInputPause(bool paused)
    {
        if (paused)
        {
            // Center the mouse in game window
            double centerX = m_WindowSize.x / 2.0;
            double centerY = m_WindowSize.y / 2.0;
            glfwSetCursorPos(GameApplication->GameWindow, centerX, centerY);

            // Reset the last mouse position to the center
            m_LastX = centerX;
            m_LastY = centerY;

            m_FirstMouse = true;
        }
        else
        {
            // Reset the last mouse positions to the current cursor position
            double xpos, ypos;
            glfwGetCursorPos(GameApplication->GameWindow, &xpos, &ypos);
            m_LastX = xpos;
            m_LastY = ypos;
        }
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(m_FOV), m_Aspect, m_NearPlane, m_FarPlane);
    }

    glm::vec3 Camera::GetForwardVector() const
    {
        return glm::normalize(m_Front);
    }

    std::string Camera::GetCardinalDirection() const
    {
        glm::vec3 forward = GetForwardVector();

        if (abs(forward.x) > abs(forward.z))
        {
            return forward.x > 0 ? "East (+X)" : "West (-X)";
        }
        else
        {
            return forward.z > 0 ? "South (+Z)" : "North (-Z)";
        }
    }

    std::string Camera::GetWorldLocationString() const
    {
        return std::format("X: {}, Y: {}, Z: {}", std::round(m_Position.x * 100.0f) / 100.0f, std::round(m_Position.y * 100.0f) / 100.0f, std::round(m_Position.z * 100.0f) / 100.0f);
    }

    void Camera::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        UpdateCameraVectors();
    }

    void Camera::SetFOV(float FOV)
    {
        m_FOV = FOV;
    }

    void Camera::UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_Up));
    }
} // BlockGame