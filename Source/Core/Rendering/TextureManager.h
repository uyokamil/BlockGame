//
// Created by physi on 19/12/2024.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "../Blocks/Block.h"

namespace OpenGLClasses
{
    class Texture;
}

namespace BlockGame {

    using namespace OpenGLClasses;

    class TextureManager {
    public:
        /// Initialize and load texture atlas
        static void Initialize();

        /// Get an existing texture by name
        static Texture* GetTexture(const std::string& name);

    private:
        /// Cache of loaded textures
        static inline std::unordered_map<std::string, std::unique_ptr<Texture>> s_TextureCache;

        /// Fallback texture
        static inline std::unique_ptr<Texture> s_FallbackTexture;

        /// Load and cache a texture
        static void LoadTexture(const std::string& name, const std::string& path);

        /// Load the fallback texture
        static void LoadFallbackTexture();
    };

} // BlockGame

#endif //TEXTUREMANAGER_H
