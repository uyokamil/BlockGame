//
// Created by physi on 19/12/2024.
//

#include "Block.h"

#include "../Utils/Maths.h"

namespace BlockGame
{
    Block::Block()
        : type(BlockType::Air),
          isOpaque(false),
          isSolid(false),
          rotation(0),
          hasRotation(false),
          minBounds(0.0f, 0.0f, 0.0f),
          maxBounds(1.0f, 1.0f, 1.0f)
    {
        textures[Front] = Maths::ToLower(BlockTypeToString(type));
    }

    Block::Block(BlockType blockType = BlockType::Air, bool opaque, bool solid, uint8_t rot, bool rotatable)
    : type(blockType), isOpaque(opaque), isSolid(solid), rotation(rot), hasRotation(rotatable),
          minBounds(0.0f, 0.0f, 0.0f),
          maxBounds(1.0f, 1.0f, 1.0f)
    {
        textures[Front] = Maths::ToLower(BlockTypeToString(type));
    }

    Block::Block(BlockType blockType, std::unordered_map<BlockFaceType, std::string> faceTextures, bool opaque,
        bool solid, uint8_t rot, bool rotatable)
    : type(blockType), isOpaque(opaque), isSolid(solid), rotation(rot), hasRotation(rotatable),
          minBounds(0.0f, 0.0f, 0.0f),
          maxBounds(1.0f, 1.0f, 1.0f)
    {
        textures = faceTextures;
    }
}
