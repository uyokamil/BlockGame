//
// Created by physi on 16/12/2024.
//

#include "Player.h"
#include "../Application/AppEvents.h"
#include "../Application/Application.h"
#include "../Utils/Logger.h"
#include "../World/World.h"

namespace BlockGame {
    Player::Player(glm::vec2 windowSize)
        : m_Camera(90.0f, 0.1f, 1000.0f, 0.25f, windowSize)
    {
        p_bIsHoveringBlock = false;
        p_HoverBlockPosition = glm::vec3(0);
        p_HoverBlockNormal = glm::vec3(0);
        m_Position = glm::vec3(0);
    }

    Player::~Player() = default;

    void Player::Update(GLFWwindow *window, float deltaTime)
    {
        HandleInput(window, deltaTime);

        m_Camera.SetPosition(m_Position);
        m_Camera.Update(window, deltaTime);

        DoLineTrace();
    }

    void Player::SetPosition(glm::vec3 position)
    {
        m_Camera.SetPosition(position);
        m_Position = position;
    }

    void Player::HandleInput(GLFWwindow *window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Move(m_Camera.GetFront(), deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Move(-m_Camera.GetFront(), deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Move(m_Camera.GetRight(), deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Move(-m_Camera.GetRight(), deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Move((glm::vec3(0, 1, 0)), deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            Move(-(glm::vec3(0, 1, 0)), deltaTime);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            // on left click
            if (!tempLeftClickBool)
            {
                tempLeftClickBool = true;
                if (p_bIsHoveringBlock)
                {
                    Application::GetWorldInstance()->BreakBlockAt(p_HoverBlockPosition);
                }
            }
        }
        else
        {
            tempLeftClickBool = false;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            // on left click
            if (!tempRightClickBool)
            {
                tempRightClickBool = true;
                if (p_bIsHoveringBlock)
                {
                    Application::GetWorldInstance()->PlaceBlockAt((p_HoverBlockPosition + p_HoverBlockNormal), BlockType::Cobblestone);
                }
            }
        }
        else
        {
            tempRightClickBool = false;
        }
    }

    void Player::Move(const glm::vec3 &direction, float deltaTime)
    {
        m_Position += speed * direction * deltaTime;
    }

    void Player::DoLineTrace()
    {
        glm::vec3 start = m_Camera.GetPosition();
        glm::vec3 direction = glm::normalize(m_Camera.GetForwardVector());
        float maxDistance = 6.0f;

        LineTraceResult result = GameApplication->GetWorldInstance()->LineTrace(start, direction, maxDistance, 0.05f);

        if (result.HitSuccess && result.HitBlock.OwnerChunk && result.HitBlock.Block)
        {
            p_HoverBlockPosition = result.HitBlock.BlockPosition;
            p_HoverBlockNormal = result.HitNormal;
            p_bIsHoveringBlock = true;
        }
        else
        {
            p_bIsHoveringBlock = false;
        }
    }
} // BlockGame