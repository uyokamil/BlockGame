//
// Created by physi on 15/12/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "../OpenGL/OpenGL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Utils/Defaults.h"


namespace BlockGame {

    class Camera {
    public:
        Camera(float fov, float nearPlane, float farPlane, float sensitivity = 0.25f, glm::vec2 windowSize = glm::vec2(WINDOW_WIDTH, WINDOW_WIDTH));
        ~Camera();

        void Update(GLFWwindow* window, float deltaTime);
        void HandleMouseInput(double xPos, double yPos);
        void UpdateAspectRatio(float width, float height);

        void SetInputPause(bool paused);

        [[nodiscard]] glm::mat4 GetViewMatrix() const;
        [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

        [[nodiscard]] glm::vec3 GetPosition() const { return m_Position; }
        [[nodiscard]] glm::vec3 GetFront() const { return m_Front; }
        [[nodiscard]] glm::vec3 GetRight() const { return m_Right; }
        [[nodiscard]] float GetFOV() const { return m_FOV; }

        [[nodiscard]] glm::vec3 GetForwardVector() const;
        std::string GetCardinalDirection() const;
        std::string GetWorldLocationString() const;

        void SetPosition(const glm::vec3& position);
        void SetFOV(float FOV);

    private:
        void UpdateCameraVectors();

        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;

        glm::vec2 m_WindowSize;

        float m_Yaw;
        float m_Pitch;

        float m_FOV;
        float m_Aspect;
        float m_NearPlane;
        float m_FarPlane;

        float m_Sensitivity;

        bool m_FirstMouse;
        double m_LastX;
        double m_LastY;

        bool firstClick = true; // temp
    };

} // BlockGame

#endif //CAMERA_H
