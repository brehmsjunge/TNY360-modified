#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <esp_attr.h>
#include "common/Log.hpp"
#include "common/config.hpp"

EXT_RAM_BSS_ATTR Log::LineInfo logBuffer[LOG_MAX_LINES];
uint16_t logIndex = 0;
uint16_t logCount = 0;
uint8_t logIndent = 0;
static SemaphoreHandle_t logMutex = NULL;

void InitMutex()
{
    if (logMutex == NULL)
    {
        logMutex = xSemaphoreCreateMutex();
    }
}

void Log::GroupStart()
{
    InitMutex();
    xSemaphoreTake(logMutex, portMAX_DELAY);
    logIndent++;
    xSemaphoreGive(logMutex);
}

void Log::GroupEnd()
{
    InitMutex();
    xSemaphoreTake(logMutex, portMAX_DELAY);
    if (logIndent > 0) logIndent--;
    xSemaphoreGive(logMutex);
}

void Log::internal::AddV(Log::Level level, const char* tag, const char* fmt, va_list args)
{
    InitMutex();
    xSemaphoreTake(logMutex, portMAX_DELAY);

    LineInfo& curLine = logBuffer[logIndex];
    curLine.level = level;
    
    vsnprintf(curLine.message, LOG_MAX_MSG_LEN, fmt, args);
    
    logIndex = (logIndex + 1) % LOG_MAX_LINES;
    if (logCount < LOG_MAX_LINES) {
        logCount++;
    }

    uint8_t curIndent = logIndent; // capture current indent for printing

    xSemaphoreGive(logMutex);
    
#if DEBUG_MODE == 1
    // Log the message to serial for immediate feedback
    // This is useful for debugging during development
    const char* levelStr = LevelToString(level);
    printf("[%s] %*s[%s] %s\n", levelStr, (int)(curIndent * 2), "", tag, curLine.message);
#endif
}

void Log::internal::Add(Log::Level level, const char* tag, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    AddV(level, tag, fmt, args);
    va_end(args);
}

const Log::LineInfo& Log::GetLine(uint16_t index)
{
    if (index >= LOG_MAX_LINES) {
        static LineInfo emptyLine = { Log::Level::Info, "" };
        return emptyLine; // Return an empty line if index is out of bounds
    }
    // loop back in the index to get the correct line
    uint16_t realIndex = (logIndex + LOG_MAX_LINES - index - 1) % LOG_MAX_LINES;
    return logBuffer[realIndex];
}

uint16_t Log::Count()
{
    return logCount;
}

const char* Log::LevelToString(Log::Level level)
{
    switch (level)
    {
        case Level::Info: return "INF";
        case Level::Warning: return "WRN";
        case Level::Error: return "ERR";
        case Level::Debug: return "DBG";
        case Level::Success: return "SUC";
        default: return "---";
    }
}

Log::Scope::Scope()
{
    Log::GroupStart();
}

Log::Scope::Scope(const char* TAG, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Log::internal::AddV(Log::Level::Info, TAG, fmt, args);
    va_end(args);

    Log::GroupStart();
}

Log::Scope::~Scope()
{
    Log::GroupEnd();
}