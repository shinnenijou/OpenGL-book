//
// Created by YuxinLiu on 2025/5/19.
//

#ifndef DEBUG_H
#define DEBUG_H

#include "opengl.h"

#include <string>
#include <chrono>
#include <functional>

// =============================================================================
// Logger - Thread-safe asynchronous logging system
// =============================================================================

// Log levels
enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

// Initialize the logger (starts background thread)
void LoggerInit();

// Shutdown the logger (waits for all logs to be printed)
void LoggerShutdown();

// Core logging function (called via macros)
void LogMessage(LogLevel level, const char* func, int line, const char* fmt, ...);

// Convenience macros for logging
#define LOG_INFO(...)  LogMessage(LogLevel::INFO, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  LogMessage(LogLevel::WARN, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LogMessage(LogLevel::ERROR, __FUNCTION__, __LINE__, __VA_ARGS__)

// =============================================================================
// OpenGL specific utilities
// =============================================================================

void printShaderLog(GLuint shader, const char* func, int line);
void printProgramLog(GLuint program, const char* func, int line);
bool checkOpenGLError();

#define GLCall(expr)    expr;   \
if (checkOpenGLError()) exit(1)

#define SHADER_LOG(shader) printShaderLog((shader), __FUNCTION__, __LINE__)
#define PROGRAM_LOG(program) printProgramLog((program), __FUNCTION__, __LINE__)

#endif //DEBUG_H
