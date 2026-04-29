#pragma once
#include <cstdint>
#include "config.hpp"

namespace Log
{
    enum class Level : uint8_t
    {
        Info,
        Warning,
        Error,
        Debug,
        Success
    };

    struct LineInfo
    {
        Level level;
        char message[LOG_MAX_MSG_LEN];
    };

    void GroupStart();
    void GroupEnd();
    /// @brief Get a log line by index
    /// @param index Index of the log line to retrieve. 0 is the most recent line, 1 the previous one, etc.
    /// @return The log line info.
    const LineInfo& GetLine(uint16_t index);
    uint16_t Count();
    const char* LevelToString(Level level);

    class Scope
    {
    public:
        Scope();
        Scope(const char* TAG, const char* fmt, ...);
        ~Scope();

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;
    };

    namespace internal
    {
        void Add(Level level, const char* tag, const char* fmt, ...);
        void AddV(Level level, const char* tag, const char* fmt, va_list args);
    }
}

#define LOG_INFO(tag, fmt, ...)    Log::internal::Add(Log::Level::Info, tag, fmt, ##__VA_ARGS__)
#define LOG_WARNING(tag, fmt, ...) Log::internal::Add(Log::Level::Warning, tag, fmt, ##__VA_ARGS__)
#define LOG_ERROR(tag, fmt, ...)   Log::internal::Add(Log::Level::Error, tag, fmt, ##__VA_ARGS__)
#define LOG_SUCCESS(tag, fmt, ...) Log::internal::Add(Log::Level::Success, tag, fmt, ##__VA_ARGS__)

#ifdef RELEASE_MODE
    #define LOG_DEBUG(tag, fmt, ...) do {} while(0)
    #define LOG_SCOPE(tag, fmt, ...) do {} while(0)
#else
    #define LOG_DEBUG(tag, fmt, ...) Log::internal::Add(Log::Level::Debug, tag, fmt, ##__VA_ARGS__)
    #define LOG_SCOPE(tag, fmt, ...) Log::Scope _log_scope_instance(tag, fmt, ##__VA_ARGS__)
#endif