//
// Created by physi on 15/12/2024.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include <cstdint>

#include "AppEvents.h"
#include "../Rendering/CubeRenderer.h"
#include "../Utils/Defaults.h"

namespace BlockGame {
    class World;
    class Player;

    enum GameState : uint8_t
    {
        GS_MENU             = 0,
        GS_WORLD_CREATE     = 1,
        GS_PLAYING_GAME     = 2,
        GS_PAUSED_GAME      = 3
    };

    class Application {
    public:
        Application();
        ~Application();

        GLFWwindow* GameWindow;

        void Update();

        glm::ivec2 GetScreenSize() const { return m_ScreenSize; }

        [[nodiscard]] GameState GetState() const { return m_GameState; }
        [[nodiscard]] static World* GetWorldInstance();

    private:
        World* m_World;

        void OnEvent(AppEvents::AppEvent event);
        void PollEvents();

        std::vector<AppEvents::AppEvent> m_AppEventQueue;
        bool m_CursorLocked = false;
        bool m_Debug = true;
        GameState m_GameState;
        glm::ivec2 m_ScreenSize = {WINDOW_WIDTH, WINDOW_HEIGHT};

        float lastFrame = 0.0f;
        float deltaTime = 0.0f;
    };

    extern Application* GameApplication;

} // BlockGame

#endif //APPLICATION_H
