// logger.h
//
// Copyright (C) 2021, Celestia Development Team
//
// Logging functions.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <iosfwd>
#include <string>

#include <fmt/format.h>

#include <celcompat/filesystem.h>

template <>
struct fmt::formatter<fs::path> : formatter<std::string>
{
    template <typename FormatContext>
    auto format(const fs::path &path, FormatContext &ctx)
    {
        return formatter<std::string>::format(path.string(), ctx);
    }
};

namespace celestia::util
{

enum class Level
{
    Error,
    Warning,
    Info,
    Verbose,
    Debug,
};

class Logger
{
 public:
    using Stream = std::basic_ostream<char>;

    Logger();
    Logger(Level level, Stream &log, Stream &err) :
        m_log(log),
        m_err(err),
        m_level(level)
    {}

    void setLevel(Level level)
    {
        m_level = level;
    }

    template <typename... Args> inline void
    debug(const char *format, const Args&... args) const;

    template <typename... Args> inline void
    info(const char *format, const Args&... args) const;

    template <typename... Args> inline void
    verbose(const char *format, const Args&... args) const;

    template <typename... Args> inline void
    warn(const char *format, const Args&... args) const;

    template <typename... Args> inline void
    error(const char *format, const Args&... args) const;

    template <typename... Args> void
    log(Level, const char *format, const Args&... args) const;

    static Logger* g_logger;

 private:
    void vlog(Level level, fmt::string_view format, fmt::format_args args) const;

    Stream &m_log;
    Stream &m_err;
    Level   m_level { Level::Info };
};

template <typename... Args> void
Logger::log(Level level, const char *format, const Args&... args) const
{
    if (level <= m_level)
        vlog(level, fmt::string_view(format), fmt::make_format_args(args...));
}

template <typename... Args> void
Logger::debug(const char *format, const Args&... args) const
{
    log(Level::Debug, format, args...);
}

template <typename... Args> inline void
Logger::info(const char *format, const Args&... args) const
{
    log(Level::Info, format, args...);
}

template <typename... Args> inline void
Logger::verbose(const char *format, const Args&... args) const
{
    log(Level::Verbose, format, args...);
}

template <typename... Args> inline void
Logger::warn(const char *format, const Args&... args) const
{
    log(Level::Warning, format, args...);
}

template <typename... Args> inline void
Logger::error(const char *format, const Args&... args) const
{
    log(Level::Error, format, args...);
}

Logger* GetLogger();
Logger* CreateLogger(Level level = Level::Info);
Logger* CreateLogger(Level level,
                     Logger::Stream &log,
                     Logger::Stream &err);
void DestroyLogger();

} // end namespace celestia::util
