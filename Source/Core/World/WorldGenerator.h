//
// Created by physi on 18/12/2024.
//

#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H
#include "../Blocks/Block.h"
#include "../Chunks/Chunk.h"

namespace BlockGame {

    Block* GetWorldBlock(const glm::vec3& position);

    void GenerateChunk(Chunk* chunk, const int WorldSeed);

} // BlockGame

#endif //WORLDGENERATOR_H
