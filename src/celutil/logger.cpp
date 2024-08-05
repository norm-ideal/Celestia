// logger.cpp
//
// Copyright (C) 2021, Celestia Development Team
//
// Logging functions.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include <iostream>

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <fmt/ostream.h>
#include "logger.h"

namespace celestia::util
{

Logger* Logger::g_logger = nullptr;

Logger* GetLogger()
{
    return Logger::g_logger;
}

Logger* CreateLogger(Level level)
{
    return CreateLogger(level, std::clog, std::cerr);
}

Logger* CreateLogger(Level level, Logger::Stream &log, Logger::Stream &err)
{
    if (Logger::g_logger == nullptr)
        Logger::g_logger = new Logger(level, log, err);
    return Logger::g_logger;
}

void DestroyLogger()
{
    delete Logger::g_logger;
}

Logger::Logger() :
    m_log(std::clog),
    m_err(std::cerr)
{
}

void Logger::vlog(Level level, fmt::string_view format, fmt::format_args args) const
{
#ifdef _MSC_VER
    if (level == Level::Debug && IsDebuggerPresent())
    {
        OutputDebugStringA(fmt::vformat(format, args).c_str());
        return;
    }
#endif

    auto &stream = (level <= Level::Warning || level == Level::Debug) ? m_err : m_log;
    fmt::vprint(stream, format, args);
}

} // end namespace celestia::util
