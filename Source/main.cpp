/*
-- BlockGame --
Version: v0.0

-- By Kamil Czarnecki --
Copyright (c) 2024-2025
Contact: kamilpczarnecki@gmail.com
https://uyokamil.github.io/
*/

#include "Core/Utils/Logger.h"
#include "Core/Application/Application.h"
#include "Core/Chunks/Chunk.h"

BlockGame::Application* BlockGame::GameApplication = nullptr;

int main()
{
    // Initialize the logger
    Logger::Init();

    // Initialize application
    BlockGame::GameApplication = new BlockGame::Application();

    // Main Application Loop
    while (!glfwWindowShouldClose(BlockGame::GameApplication->GameWindow))
    {
        BlockGame::GameApplication->Update();
    }

    delete BlockGame::GameApplication;
    return 0;
}
