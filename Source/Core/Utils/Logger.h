//
// Created by Kamil Czarnecki on 15/12/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iomanip>
#include <iostream>
#include <string>
#include <windows.h>
#include <format>

class Logger {
private:
    static HANDLE console;

    static void SetColor(int color) {
        SetConsoleTextAttribute(console, color);
    }

    static void ResetColor() {
        SetConsoleTextAttribute(console, 15); // Reset to white
    }

    static std::string GetTimestamp() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(&tm, "[%H:%M:%S]");
        return ss.str();
    }

public:
    static void Init() {
        console = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    static void Debug(const std::string& message) {
        SetColor(15); // White
        std::cout << GetTimestamp() << " [DEBUG] " << message << std::endl;
        ResetColor();
    }

    static void Warning(const std::string& message) {
        SetColor(14); // Yellow
        std::cout << GetTimestamp() << " [WARNING] " << message << std::endl;
        ResetColor();
    }

    static void Error(const std::string& message) {
        SetColor(12); // Light Red
        std::cout << GetTimestamp() << " [ERROR] " << message << std::endl;
        ResetColor();
    }

    static void Fatal(const std::string& message) {
        SetColor(4); // Dark Red
        std::cout << GetTimestamp() << " [FATAL] " << message << std::endl;
        ResetColor();
    }
};

#endif //LOGGER_H
