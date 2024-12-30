//
// Created by physi on 15/12/2024.
//

#ifndef BLOCK_H
#define BLOCK_H
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "BlockDatabase.h"
#include "../Utils/Defaults.h"

namespace BlockGame {
    enum class BlockType : uint8_t;

    enum BlockFaceType : uint8_t
    {
        Back     = 0,
        Front    = 1,
        Left     = 2,
        Right    = 3,
        Bottom   = 4,
        Top      = 5
    };

    // TODO: Remove this temp or make it better.
    // Function to get the string representation of the enum
    inline std::string BlockFaceTypeToString(BlockFaceType faceType) {
        switch (faceType) {
            case Top:    return "Top";
            case Bottom: return "Bottom";
            case Left:   return "Left";
            case Right:  return "Right";
            case Front:  return "Front";
            case Back:   return "Back";
            default:     return "Unknown";
        }
    }

    struct Block {
        BlockType type;     // Type of block

        // AABB bounds relative to the block's position
        glm::vec3 minBounds;
        glm::vec3 maxBounds;

        // Additional textures
        std::unordered_map<BlockFaceType, std::string> textures;

        // Properties
        bool isOpaque = false;
        bool isSolid = false;

        // Rotation and special flags
        uint8_t rotation = 0;
        bool hasRotation = false;

        // Light
        uint8_t lightLevel = 0;

        // Constructor
        Block();

        Block(BlockType blockType,
                       bool opaque = true,
                       bool solid = true,
                       uint8_t rot = 0,
                       bool rotatable = false);

        Block(BlockType blockType,
                       std::unordered_map<BlockFaceType, std::string> faceTextures,
                       bool opaque = true,
                       bool solid = true,
                       uint8_t rot = 0,
                       bool rotatable = false);

        // Determines which texture to use based on face type
        [[nodiscard]] std::string GetTextureKey(BlockFaceType face) const {
            switch (face) {
                case Top:
                {
                    if (textures.contains(Top))
                    {
                        return textures.at(Top);
                    }
                    break;
                }
                case Bottom:
                {
                    if (textures.contains(Bottom))
                    {
                        return textures.at(Bottom);
                    }
                    break;
                }
                case Front:
                {
                    if (textures.contains(Front))
                    {
                        return textures.at(Front);
                    }
                    break;
                }
            }
            return textures.at(Front);
        }
    };
} // BlockGame

#endif //BLOCK_H
