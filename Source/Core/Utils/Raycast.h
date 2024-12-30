//
// Created by physi on 18/12/2024.
//

#ifndef RAYCAST_H
#define RAYCAST_H
#include <vector>
#include <glm/vec3.hpp>

namespace BlockGame {

    void FastVoxelTraversal(glm::vec3 rayStart, glm::vec3 rayEnd, std::vector<glm::vec3>& visitedBlocks);

} // BlockGame

#endif //RAYCAST_H
