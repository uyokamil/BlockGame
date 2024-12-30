//
// Created by physi on 18/12/2024.
//

#ifndef FRUSTUM_H
#define FRUSTUM_H
#include <array>
#include <glm/glm.hpp>

namespace BlockGame {

    struct FrustumAABB {
        FrustumAABB(const glm::vec3& dimensions, const glm::vec3& position)
            : Dimensions(dimensions), Position(position) {}

        const glm::vec3& Dimensions;
        const glm::vec3& Position;

        glm::vec3 GetFarthestPoint(const glm::vec3& normal) const;
        glm::vec3 GetNearestPoint(const glm::vec3& normal) const;
    };

    struct Plane {
        float SignedDistanceTo(const glm::vec3& point) const;

        float DistanceToOrigin;
        glm::vec3 Normal;
    };

    enum Planes
    {
        NearPlane    = 0,
        FarPlane     = 1,
        LeftPlane    = 2,
        RightPlane   = 3,
        TopPlane     = 4,
        BottomPlane  = 5
    };

    class ViewFrustum {
    public:
        void Update(const glm::mat4& viewMatrix);
        bool IsBoxIntersectingFrustum(const FrustumAABB& box) const;

    private:
        std::array<Plane, 6> m_Planes;
    };

} // BlockGame

#endif //FRUSTUM_H
