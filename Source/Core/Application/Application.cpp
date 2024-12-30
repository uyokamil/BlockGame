//
// Created by physi on 15/12/2024.
//

#include <format>

#include "Application.h"

#include <bits/stl_algo.h>

#include "../Rendering/CubeRenderer.h"
#include "../Utils/Logger.h"
#include "../Utils/Defaults.h"
#include "../World/World.h"

/*
    The OpenGL Debug callback
*/
void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* msg, const void* data)
{
    char* _source;
    char* _type;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = (char*)"API";
        break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = (char*)"WINDOW SYSTEM";
        break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = (char*)"SHADER COMPILER";
        break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = (char*)"THIRD PARTY";
        break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = (char*)"APPLICATION";
        break;

        case GL_DEBUG_SOURCE_OTHER:
        default:
            _source = (char*)"UNKNOWN";
        break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            _type = (char*)"ERROR";
        break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = (char*)"DEPRECATED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = (char*)"UDEFINED BEHAVIOR";
        break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = (char*)"PORTABILITY";
        break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = (char*)"PERFORMANCE";
        return;
        break;

        case GL_DEBUG_TYPE_OTHER:
            _type = (char*)"OTHER";
        break;

        case GL_DEBUG_TYPE_MARKER:
            _type = (char*)"MARKER";
        break;

        default:
            _type = (char*)"UNKNOWN";
        break;
    }

    // printf("%d: %s of %s severity, raised from %s: %s\n",id, _type, _severity, _source, msg);
    std::string result = std::format("{}: {}, raised from {}: {}",
    id, _type, _source, msg);

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Logger::Error(result);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Logger::Warning(result);
            break;
        case GL_DEBUG_SEVERITY_LOW:
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            Logger::Debug(result);
            break;
    }
}

namespace BlockGame {

    static void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error: %s\n", description);
    }

    Application::Application() : m_GameState(GameState::GS_MENU)
    {
        Logger::Debug("Starting Game Application");
        glfwSetErrorCallback(glfwErrorCallback);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

        GameWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Yonnie!!!<3", nullptr, nullptr);

        if (GameWindow == nullptr)
        {
            Logger::Fatal("Failed to create GLFW window!");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(GameWindow);

        // V-Sync Enable
        glfwSwapInterval(1);

        gladLoadGL();
        glViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);

        if (m_Debug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

        char* renderer = (char*)glGetString(GL_RENDERER);
        char* version = (char*)glGetString(GL_VERSION);

        Logger::Debug(
        std::format("Renderer {} started, version {}.", renderer, version)
        );

        // Set mouse input mode to "normal" at start
        m_CursorLocked = false;
        glfwSetInputMode(GameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        AppEvents::InitAppEventsSystem(GameWindow, &m_AppEventQueue);

        // Enable DepthTest and alpha blending
        glEnable(GL_DEPTH_TEST | GL_BLEND);
        glDepthMask(GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // TODO:
        m_World = new World(0, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), "WorldName");

        m_GameState = GameState::GS_PLAYING_GAME;
    }

    Application::~Application()
    {
        delete m_World;
        glfwDestroyWindow(GameWindow);
        glfwTerminate();
    }

    // Main Application Loop
    void Application::Update()
    {
        PollEvents();

        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Enable DepthTest and alpha blending
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.44f, 0.78f, 0.88f, 1.0f);

        // Clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_GameState == GameState::GS_PLAYING_GAME)
        {
            // Update world
            m_World->Update(GameWindow, deltaTime);

            // Render world
            m_World->RenderWorld();
        }

        // Swap buffers
        glfwSwapBuffers(GameWindow);
        // Poll GLFW events
        glfwPollEvents();
    }

    World* Application::GetWorldInstance()
    {
        return GameApplication->m_World;
    }

    void Application::OnEvent(AppEvents::AppEvent event)
    {
        switch (event.type)
        {
            case AppEvents::MouseDown:
            {
                // gui mouse callback
                break;
            }
            case AppEvents::MouseUp:
            {
                // gui mouse callback
                break;
            }

            case AppEvents::WindowResize:
            {
                int w, h;
                glfwGetFramebufferSize(GameWindow, &w, &h);

                if (w <= 0 || h <= 0) return;  // Ignore invalid sizes

                // Update OpenGL viewport
                glViewport(0, 0, w, h);

                // Update screen size
                m_ScreenSize = glm::ivec2(w, h);

                event.windowWidth = m_ScreenSize.x;
                event.windowHeight = m_ScreenSize.y;

                break;
            }

            case AppEvents::KeyDown:
            {
                if (event.key == GLFW_KEY_ESCAPE && m_GameState == GameState::GS_PLAYING_GAME)
                {
                    m_GameState = GameState::GS_PAUSED_GAME;

                    // Unlock mouse from game
                    glfwSetInputMode(GameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                else if (event.key == GLFW_KEY_ESCAPE && m_GameState == GameState::GS_PAUSED_GAME)
                {
                    m_GameState = GameState::GS_PLAYING_GAME;

                    // Lock mouse into game
                    glfwSetInputMode(GameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                else if (event.key == GLFW_KEY_F3 && m_GameState == GameState::GS_PLAYING_GAME)
                {
                    // show debug info
                }
                break;
            }
        }

        if (m_World)
        {
            m_World->OnEvent(event);
        }
    }

    void Application::PollEvents()
    {
        for (int i = 0; i < m_AppEventQueue.size(); i++)
        {
            OnEvent(m_AppEventQueue[i]);
        }

        m_AppEventQueue.clear();
    }
} // BlockGame