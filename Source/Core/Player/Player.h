//
// Created by physi on 16/12/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "../OpenGL/OpenGL.h"

#include "Camera.h"

namespace BlockGame {
    namespace AppEvents
    {
        struct AppEvent;
    }

    class Player {
    public:
        Player(glm::vec2 windowSize);
        ~Player();

        void Update(GLFWwindow* window, float deltaTime);
        void OnEvent(AppEvents::AppEvent event);

        void SetPosition(glm::vec3 position);
        glm::vec3 GetPosition() const { return m_Position; }

        Camera& GetCamera() { return m_Camera; }

        bool p_bIsHoveringBlock;
        glm::vec3 p_HoverBlockPosition;
        glm::vec3 p_HoverBlockNormal;

    private:
        float speed = 8.0f;

        glm::vec3 m_Position;

        Camera m_Camera;

        void HandleInput(GLFWwindow* window, float deltaTime);
        void Move(const glm::vec3& direction, float deltaTime);

        void DoLineTrace();

        bool tempLeftClickBool = false;
        bool tempRightClickBool = false;
    };

} // BlockGame

#endif //PLAYER_H
