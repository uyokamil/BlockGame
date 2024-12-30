//
// Created by physi on 19/12/2024.
//

#ifndef TIMER_H
#define TIMER_H
#include <chrono>

#include "Logger.h"

namespace BlockGame {

    class Timer {
    public:
        Timer()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            Stop();
        }

        void Stop()
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
            Logger::Debug(std::format("Timer: Timer elapsed {}ms", elapsed));
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    };

} // BlockGame

#endif //TIMER_H
