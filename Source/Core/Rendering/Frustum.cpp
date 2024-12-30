//
// Created by physi on 18/12/2024.
//

#include "Frustum.h"

namespace BlockGame {
    glm::vec3 FrustumAABB::GetFarthestPoint(const glm::vec3 &normal) const
    {
        glm::vec3 furthest = Position;

        if (normal.x < 0) furthest.x += Dimensions.x;
        if (normal.y < 0) furthest.y += Dimensions.y;
        if (normal.z < 0) furthest.z += Dimensions.z;

        return furthest;
    }

    glm::vec3 FrustumAABB::GetNearestPoint(const glm::vec3 &normal) const
    {
        glm::vec3 nearest = Position;

        if (normal.x > 0) nearest.x += Dimensions.x;
        if (normal.y > 0) nearest.y += Dimensions.y;
        if (normal.z > 0) nearest.z += Dimensions.z;

        return nearest;
    }

    float Plane::SignedDistanceTo(const glm::vec3& point) const
    {
        return glm::dot(point, Normal) + DistanceToOrigin;
    }

    void ViewFrustum::Update(const glm::mat4 &viewMatrix)
    {
        m_Planes[Planes::LeftPlane].Normal.x = viewMatrix[0][3] + viewMatrix[0][0];
        m_Planes[Planes::LeftPlane].Normal.y = viewMatrix[1][3] + viewMatrix[1][0];
        m_Planes[Planes::LeftPlane].Normal.z = viewMatrix[2][3] + viewMatrix[2][0];
        m_Planes[Planes::LeftPlane].DistanceToOrigin = viewMatrix[3][3] + viewMatrix[3][0];

        m_Planes[Planes::RightPlane].Normal.x = viewMatrix[0][3] - viewMatrix[0][0];
        m_Planes[Planes::RightPlane].Normal.y = viewMatrix[1][3] - viewMatrix[1][0];
        m_Planes[Planes::RightPlane].Normal.z = viewMatrix[2][3] - viewMatrix[2][0];
        m_Planes[Planes::RightPlane].DistanceToOrigin = viewMatrix[3][3] - viewMatrix[3][0];

        m_Planes[Planes::BottomPlane].Normal.x = viewMatrix[0][3] + viewMatrix[0][1];
        m_Planes[Planes::BottomPlane].Normal.y = viewMatrix[1][3] + viewMatrix[1][1];
        m_Planes[Planes::BottomPlane].Normal.z = viewMatrix[2][3] + viewMatrix[2][1];
        m_Planes[Planes::BottomPlane].DistanceToOrigin = viewMatrix[3][3] + viewMatrix[3][1];

        m_Planes[Planes::TopPlane].Normal.x = viewMatrix[0][3] - viewMatrix[0][1];
        m_Planes[Planes::TopPlane].Normal.y = viewMatrix[1][3] - viewMatrix[1][1];
        m_Planes[Planes::TopPlane].Normal.z = viewMatrix[2][3] - viewMatrix[2][1];
        m_Planes[Planes::TopPlane].DistanceToOrigin = viewMatrix[3][3] - viewMatrix[3][1];

        m_Planes[Planes::NearPlane].Normal.x = viewMatrix[0][3] + viewMatrix[0][2];
        m_Planes[Planes::NearPlane].Normal.y = viewMatrix[1][3] + viewMatrix[1][2];
        m_Planes[Planes::NearPlane].Normal.z = viewMatrix[2][3] + viewMatrix[2][2];
        m_Planes[Planes::NearPlane].DistanceToOrigin = viewMatrix[3][3] + viewMatrix[3][2];

        m_Planes[Planes::FarPlane].Normal.x = viewMatrix[0][3] - viewMatrix[0][2];
        m_Planes[Planes::FarPlane].Normal.y = viewMatrix[1][3] - viewMatrix[1][2];
        m_Planes[Planes::FarPlane].Normal.z = viewMatrix[2][3] - viewMatrix[2][2];
        m_Planes[Planes::FarPlane].DistanceToOrigin = viewMatrix[3][3] - viewMatrix[3][2];

        for (auto&[DistanceToOrigin, Normal] : m_Planes)
        {
            const float length = glm::length(Normal);
            Normal /= length;
            DistanceToOrigin /= length;
        }
    }

    bool ViewFrustum::IsBoxIntersectingFrustum(const FrustumAABB &box) const
    {
        for (auto& plane : m_Planes)
        {
            if (plane.SignedDistanceTo(box.GetNearestPoint(plane.Normal)) < 0)
            {
                return false; // Fully outside
            }

            // Not needed for now. We just want a yes or no.
            // if (plane.SignedDistanceTo(box.GetFarthestPoint(plane.Normal)) >= 0)
            // {
            //     continue;  // At least partially inside
            // }
        }

        // Return true if NOT outside.
        // This will return true if intesecting, or if fully inside. Perfect.
        return true;
    }
} // BlockGame