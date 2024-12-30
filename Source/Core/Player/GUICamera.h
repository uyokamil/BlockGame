//
// Created by physi on 27/12/2024.
//

#ifndef ORTHOHRAPHICCAMERA_H
#define ORTHOHRAPHICCAMERA_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BlockGame {

    class GUICamera {
    public:
        GUICamera(float left, float right, float bottom, float top);
        ~GUICamera();

        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

        void SetRotation(float rotation)
        {
            m_Rotation = rotation;
            RecalculateViewMatrix();
        }

        void SetScale(const glm::vec3& scaleFactor)
        {
            m_ScaleFactor = scaleFactor;
            RecalculateViewMatrix();
        }

        float GetRotation() const { return m_Rotation; }
        const glm::vec4& GetProjectionCoords() const { return m_ProjectionCoords; }
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        const glm::vec3& GetScaleFactor() const { return m_ScaleFactor; }
        const glm::mat4& GetTransformMatrix() const { return m_TransformMatrix; }

    private:
        void RecalculateViewMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::mat4 m_TransformMatrix;

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec4 m_ProjectionCoords = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec3 m_ScaleFactor = { 1.0f, 1.0f, 1.0f };

        float m_Rotation = 0.0f;

    };

} // BlockGame

#endif //ORTHOHRAPHICCAMERA_H
