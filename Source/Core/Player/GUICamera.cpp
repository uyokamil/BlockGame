//
// Created by physi on 27/12/2024.
//

#include "GUICamera.h"

namespace BlockGame
{
    // Constructor for OrthographicCamera: Initializes the camera with bounds (left, right, bottom, top)
    GUICamera::GUICamera(float left, float right, float bottom, float top)
    {
        // Identity matrix for the view transformation (1.0 for uniform scale)
        m_ViewMatrix = glm::mat4(1.0f);

        // Create the projection matrix based on bounds
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

        // Initialize the combined ViewProjection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix;

        // Store the projection bounds
        m_ProjectionCoords = {left, right, bottom, top};
    }

    GUICamera::~GUICamera() = default;

    // Update the projection bounds and recalculate the projection matrix
    void GUICamera::SetProjection(float left, float right, float bottom, float top)
    {
        // Update stored projection bounds
        m_ProjectionCoords = {left, right, bottom, top};

        // Create new projection matrix
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

        // Update the combined ViewProjection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    // Method to recalculate the view matrix based on camera position, rotation, and scale
    void GUICamera::RecalculateViewMatrix()
    {
        // Create a transformation matrix based on position, rotation, and scaling
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        // Invert the transform to go from world space to view space
        transform = glm::inverse(transform);

        // Scale the transformation (e.g., for zooming in/out)
        transform = glm::scale(glm::mat4(transform), glm::vec3(m_ScaleFactor));

        // Update the view matrix with the calculated transform
        m_ViewMatrix = transform;

        // Recalculate the combined ViewProjection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
} // BlockGame