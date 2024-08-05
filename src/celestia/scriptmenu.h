// scriptmenu.h
//
// Copyright (C) 2007, Chris Laurel <claurel@shatters.net>
//
// Scan a directory and build a list of Celestia script files.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include <string>
#include <vector>

#include <celcompat/filesystem.h>

struct ScriptMenuItem
{
    fs::path filename;
    std::string title;
};

std::vector<ScriptMenuItem>
ScanScriptsDirectory(const fs::path &dirname, bool deep);
