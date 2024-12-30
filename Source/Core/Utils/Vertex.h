//
// Created by physi on 17/12/2024.
//

#ifndef VERTEX_H
#define VERTEX_H

#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "Defaults.h"

namespace BlockGame
{
#pragma pack(push, 1) // Pack and disable padding
    struct Vertex
    {
        glm::i8vec3 position;
        glm::i16vec2 texCoords;
        uint8_t lightLevel;
        uint8_t blockFaceLighting;
    };
#pragma pack(pop)
}

#endif //VERTEX_H
