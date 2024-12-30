//
// Created by physi on 15/12/2024.
//

#ifndef APPEVENTS_H
#define APPEVENTS_H
#include <vector>

#include "../OpenGL/OpenGL.h"

namespace BlockGame {

    namespace AppEvents
    {
        enum EventType
        {
            KeyDown      = 0,
            KeyUp        = 1,
            MouseDown    = 2,
            MouseUp      = 3,
            MouseScroll  = 4,
            MouseMove    = 5,
            WindowResize = 6,
            Undefined
        };

        struct AppEvent
        {
            EventType type;

            GLFWwindow* window;
            int key;
            int button;
            int modifiers;
            int windowWidth, windowHeight;
            double mouseX, mouseY;
            double mouseScrollX, mouseScrollY;
            double timeStep;
        };

        void InitAppEventsSystem(GLFWwindow* window, std::vector<AppEvent>* appEventQueue);
        void QueueAppEvent(AppEvent appEvent);

    } // AppEvents
} // BlockGame

#endif //APPEVENTS_H
