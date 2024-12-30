//
// Created by physi on 18/12/2024.
//

#ifndef BLOCKDATABASE_H
#define BLOCKDATABASE_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Block.h"

namespace BlockGame {

    using namespace std;

    struct Block;

    enum class BlockType : uint8_t
    {
        // BLOCK           / BLOCK ID //
        Air                    = 0,
        Stone                  = 1,
        Grass                  = 2,
        Dirt                   = 3,
        Cobblestone            = 4,

        MAX
    };

    // TODO: Remove this temp or make it better.
    // Function to get the string representation of the enum
    inline std::string BlockTypeToString(BlockType blockType) {
        switch (blockType) {
            case BlockType::Air:    return "Air";
            case BlockType::Stone: return "Stone";
            case BlockType::Grass:   return "Grass";
            case BlockType::Dirt:  return "Dirt";
            case BlockType::Cobblestone:  return "Cobblestone";
            default:     return "Unknown";
        }
    }

    class BlockDatabase {
    public:
        static void Initialize();
        static const Block& GetBlock(BlockType blockType);

    private:
        static inline unordered_map<BlockType, Block> s_BlockRegistry;
    };

} // BlockGame

#endif //BLOCKDATABASE_H
