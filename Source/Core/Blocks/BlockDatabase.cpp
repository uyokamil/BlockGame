//
// Created by physi on 18/12/2024.
//

#include "BlockDatabase.h"

#include <format>

#include "../OpenGL/Texture.h"

#include "Block.h"
#include "../Utils/Logger.h"

namespace BlockGame {

    void BlockDatabase::Initialize()
    {
        constexpr float TILE_SIZE = 1.0f / 16.0f;  // Each tile is 1/16th of the atlas

        // Register common blocks
        s_BlockRegistry = {
            { BlockType::Air, Block(BlockType::Air,  false, false) },  // Air: no texture, non-solid
            { BlockType::Stone, Block(BlockType::Stone)},

            { BlockType::Grass, Block(BlockType::Grass, {
            {BlockFaceType::Bottom, "dirt"},
            {BlockFaceType::Left, "grass_side"},
            {BlockFaceType::Right, "grass_side"},
            {BlockFaceType::Front, "grass_side"},
            {BlockFaceType::Back, "grass_side"},
            {BlockFaceType::Top, "grass_top"}
            })},

            { BlockType::Dirt, Block(BlockType::Dirt)},
            { BlockType::Cobblestone, Block(BlockType::Cobblestone)}
        };
    }

    const Block& BlockDatabase::GetBlock(BlockType blockType)
    {
        auto it = s_BlockRegistry.find(blockType);
        if (it != s_BlockRegistry.end()) {
            return it->second;
        }

        // Log error and return a default block to prevent crashes
        Logger::Error(std::format("Block type {} not found in registry!", static_cast<int>(blockType)));
        static Block defaultBlock(BlockType::Air);  // Air block as fallback
        return defaultBlock;
    }
} // BlockGame