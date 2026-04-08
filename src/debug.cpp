#include "debug.h"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

// =============================================================================
// Logger Implementation
// =============================================================================

struct LogEntry {
    LogLevel level;
    std::string timestamp;
    std::string func;
    int line;
    std::string message;
};

class Logger {
public:
    Logger() : running_(true), thread_(&Logger::WorkerThread, this) {}

    ~Logger() {
        Log(LogLevel::INFO, __FUNCTION__, __LINE__, "logger shutdown", {});
        Shutdown();
    }

    void Log(LogLevel level, const char* func, int line, const char* fmt, va_list args) {
        // Format the user message
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, args);

        LogEntry entry;
        entry.level = level;
        entry.timestamp = GetCurrentTimestamp();
        entry.func = func;
        entry.line = line;
        entry.message = buffer;

        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            queue_.push(std::move(entry));
        }
        cv_.notify_one();
    }

    void Shutdown() {
        if (!running_.exchange(false)) return;  // Already shut down

        cv_.notify_one();
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    void WorkerThread() {
        while (running_ || !queue_.empty()) {
            LogEntry entry;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                cv_.wait(lock, [this] { return !queue_.empty() || !running_; });

                if (queue_.empty() && !running_) break;

                if (!queue_.empty()) {
                    entry = std::move(queue_.front());
                    queue_.pop();
                }
            }

            if (!entry.message.empty()) {
                PrintEntry(entry);
            }
        }
    }

    void PrintEntry(const LogEntry& entry) {
        const char* levelStr = "";
        FILE* output = stdout;

        switch (entry.level) {
            case LogLevel::INFO:  levelStr = "INFO"; break;
            case LogLevel::WARN:  levelStr = "WARN"; output = stderr; break;
            case LogLevel::ERROR: levelStr = "ERROR"; output = stderr; break;
        }

        fprintf(output, "[%s] [%s] %s:%d - %s\n",
                entry.timestamp.c_str(),
                levelStr,
                entry.func.c_str(),
                entry.line,
                entry.message.c_str());
        fflush(output);
    }

    static std::string GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::ostringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    std::queue<LogEntry> queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
    std::thread thread_;
};


void LogMessage(LogLevel level, const char* func, int line, const char* fmt, ...) {
    static Logger logger{};
    va_list args;
    va_start(args, fmt);
    logger.Log(level, func, line, fmt, args);
    va_end(args);
}

// =============================================================================
// OpenGL specific utilities
// =============================================================================

void printShaderLog(GLuint shader, const char* func, int line) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        int written = 0;
        char* log = new char[len];
        glGetShaderInfoLog(shader, len, &written, log);
        LogMessage(LogLevel::INFO, func, line, "Shader Info: %s", log);
        delete[] log;
    }
}

void printProgramLog(GLuint program, const char* func, int line) {
    int len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        int written = 0;
        char* log = new char[len];
        glGetProgramInfoLog(program, len, &written, log);
        LogMessage(LogLevel::INFO, func, line, "Program Info: %s", log);
        delete[] log;
    }
}

bool checkOpenGLError() {
    bool foundError = false;

    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        foundError = true;
        const char* errStr = "";
        switch (err) {
            case GL_INVALID_ENUM:                  errStr = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 errStr = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             errStr = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                 errStr = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               errStr = "UNKNOWN"; break;
        }
        LOG_ERROR("OpenGL Error: %s (0x%X)", errStr, err);
    }

    return foundError;
}
