//
// Created by physi on 19/12/2024.
//

#include "TextureManager.h"

#include <format>

#include "../OpenGL/Texture.h"
#include "../Utils/Logger.h"

namespace BlockGame {
    void TextureManager::Initialize()
    {
        // Load the fallback texture
        LoadFallbackTexture();

        // Load textures
        LoadTexture("stone", std::string(RESOURCE_PATH) + "/Textures/Blocks/stone.png");
        LoadTexture("grass_top", std::string(RESOURCE_PATH) + "/Textures/Blocks/grass_top.png");
        LoadTexture("grass_side", std::string(RESOURCE_PATH) + "/Textures/Blocks/grass_side.png");
        LoadTexture("dirt", std::string(RESOURCE_PATH) + "/Textures/Blocks/dirt.png");
        LoadTexture("cobblestone", std::string(RESOURCE_PATH) + "/Textures/Blocks/cobblestone.png");
    }

    void TextureManager::LoadFallbackTexture()
    {
        if (!s_FallbackTexture) {
            s_FallbackTexture = std::make_unique<Texture>();
            s_FallbackTexture->CreateTexture(std::string(RESOURCE_PATH) + "/Textures/default.png");
            Logger::Debug("Fallback texture loaded.");
        }
    }

    void TextureManager::LoadTexture(const std::string& name, const std::string& path)
    {
        // Check if the texture is already loaded
        if (s_TextureCache.contains(name)) {
            Logger::Warning(std::format("Texture '{}' is already loaded!", name));
            return;
        }

        // Create and load the texture
        auto texture = std::make_unique<Texture>();
        texture->CreateTexture(path);

        if (texture->GetTextureID() == 0) {
            Logger::Error(std::format("Failed to load texture '{}' from path '{}'.", name, path));
            return;
        }

        s_TextureCache[name] = std::move(texture);
        Logger::Debug(std::format("Texture '{}' loaded from '{}'.", name, path));
    }

    Texture* TextureManager::GetTexture(const std::string &name)
    {
        const auto it = s_TextureCache.find(name);
        if (it != s_TextureCache.end()) {
            return it->second.get();
        }

        //Logger::Error(std::format("Texture '{}' not found. Returning fallback texture.", name));
        return s_FallbackTexture.get();
    }
} // BlockGame