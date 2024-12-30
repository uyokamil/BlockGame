cmake_minimum_required(VERSION 3.30)
project(BlockGame)

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(RESOURCE_PATH="${CMAKE_SOURCE_DIR}/Source/Resources")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(BlockGame
        Source/main.cpp
        Source/Core/Utils/Logger.h
        Source/Core/Utils/Logger.cpp
        Source/Core/Application/Application.cpp
        Source/Core/Application/Application.h
        Source/Core/Application/AppEvents.cpp
        Source/Core/Application/AppEvents.h
        Source/Core/Utils/Defaults.h
        Source/Core/Blocks/Block.h
        Source/Core/Player/Camera.cpp
        Source/Core/Player/Camera.h
        Source/Core/Player/Player.cpp
        Source/Core/Player/Player.h
        Source/Core/Chunks/Chunk.cpp
        Source/Core/Chunks/Chunk.h
        Source/Core/OpenGL/VertexArray.cpp
        Source/Core/OpenGL/VertexArray.h
        Source/Core/OpenGL/VertexBuffer.cpp
        Source/Core/OpenGL/VertexBuffer.h
        Source/Core/Rendering/CubeRenderer.cpp
        Source/Core/Rendering/CubeRenderer.h
        Source/Core/OpenGL/IndexBuffer.cpp
        Source/Core/OpenGL/IndexBuffer.h
        Source/Core/OpenGL/Shader.cpp
        Source/Core/OpenGL/Shader.h
        Source/Core/OpenGL/Texture.cpp
        Source/Core/OpenGL/Texture.h
        Source/Core/OpenGL/OpenGL.h
        Source/Core/OpenGL/stb_image.cpp
        Source/Core/OpenGL/stb_image.h
        Source/Core/Utils/Vertex.h
        Source/Core/Rendering/Frustum.cpp
        Source/Core/Rendering/Frustum.h
        Source/Core/Blocks/BlockDatabase.cpp
        Source/Core/Blocks/BlockDatabase.h
        Source/Core/Utils/Raycast.cpp
        Source/Core/Utils/Raycast.h
        Source/Core/World/World.cpp
        Source/Core/World/World.h
        Source/Core/Rendering/Renderer.cpp
        Source/Core/Rendering/Renderer.h
        Source/Core/World/WorldGenerator.cpp
        Source/Core/World/WorldGenerator.h
        Source/Core/Utils/Timer.h
        Source/Core/Rendering/TextureManager.cpp
        Source/Core/Rendering/TextureManager.h
        Source/Core/Blocks/Block.cpp
        Source/Core/Utils/Maths.cpp
        Source/Core/Utils/Maths.h
        Source/Core/Rendering/GUIRenderer.cpp
        Source/Core/Rendering/GUIRenderer.h
        Source/Core/Player/GUICamera.cpp
        Source/Core/Player/GUICamera.h
)

target_link_libraries(BlockGame PRIVATE
        glad::glad
        glfw
        glm::glm
        OpenGL::GL
)