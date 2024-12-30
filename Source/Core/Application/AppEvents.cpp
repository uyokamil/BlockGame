//
// Created by physi on 15/12/2024.
//

#include "AppEvents.h"

namespace BlockGame {

    namespace AppEvents
    {

        static std::vector<AppEvent>* AppEventQueue = nullptr;

        void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void MouseEventCallback(GLFWwindow* window, int button, int action, int mods);
        void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);
        void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
        void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

        void InitAppEventsSystem(GLFWwindow *window, std::vector<AppEvent> *appEventQueue)
        {
            AppEventQueue = appEventQueue;

            // Set all the callbacks
            glfwSetKeyCallback(window, KeyEventCallback);
            glfwSetMouseButtonCallback(window, MouseEventCallback);
            glfwSetScrollCallback(window, ScrollEventCallback);
            glfwSetCursorPosCallback(window, CursorPositionCallback);
            glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
        }

        void QueueAppEvent(AppEvent appEvent)
        {
            if (AppEventQueue)
            {
                AppEventQueue->push_back(appEvent);
            }
        }

        void KeyEventCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            AppEvent event;

            switch (action)
            {
                case GLFW_PRESS:
                    event.type = EventType::KeyDown;
                    break;
                case GLFW_RELEASE:
                    event.type = EventType::KeyUp;
                    break;
                default:
                    event.type = EventType::Undefined;
                    break;
            }

            event.window = window;
            event.key = key;
            event.modifiers = mods;

            AppEventQueue->push_back(event);
        }

        void MouseEventCallback(GLFWwindow *window, int button, int action, int mods)
        {
            AppEvent event;

            switch (action)
            {
                case GLFW_PRESS:
                    event.type = EventType::MouseDown;
                break;
                case GLFW_RELEASE:
                    event.type = EventType::MouseUp;
                break;
                default:
                    event.type = EventType::Undefined;
                break;
            }

            event.window = window;
            event.key = button;
            event.modifiers = mods;

            AppEventQueue->push_back(event);
        }

        void ScrollEventCallback(GLFWwindow *window, double xOffset, double yOffset)
        {
            AppEvent event;

            event.type = EventType::MouseScroll;
            event.window = window;
            event.mouseScrollX = xOffset;
            event.mouseScrollY = yOffset;

            AppEventQueue->push_back(event);
        }

        void CursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
        {
            AppEvent event;

            event.type = EventType::MouseMove;
            event.window = window;
            event.mouseX = xPos;
            event.mouseY = yPos;

            AppEventQueue->push_back(event);
        }

        void FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
        {
            AppEvent event;

            event.type = EventType::WindowResize;
            event.windowWidth = width;
            event.windowHeight = height;

            AppEventQueue->push_back(event);
        }

    } // AppEvents

} // BlockGame
