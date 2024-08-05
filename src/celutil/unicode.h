// unicode.h
//
// Copyright (C) 2023-present, the Celestia Development Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <string>
#include <string_view>

#include <celutil/flag.h>

namespace celestia::util
{

enum class ConversionOption : unsigned int
{
    None            = 0x00,
    ArabicShaping   = 0x01,
    BidiReordering  = 0x02,
};

ENUM_CLASS_BITWISE_OPS(ConversionOption);

bool UTF8StringToUnicodeString(std::string_view input, std::u16string &output);
bool ApplyBidiAndShaping(std::u16string_view input,
                         std::u16string &output,
                         ConversionOption options = ConversionOption::None);

}
