/* =========================================================================
 * This file is part of str-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * (C) Copyright 2022, Maxar Technologies, Inc.
 *
 * xml.lite-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CODA_OSS_str_EncodedStringView_h_INCLLUDED_
#define CODA_OSS_str_EncodedStringView_h_INCLLUDED_
#pragma once

#include <string>

#include "str/Encoding.h"

/*!
 * \file EncodedStringView.h
 * \brief A String that can be either UTF-8 or "native" 
 *
 * On Linux, there is good support for UTF-8, so a std::string encoded
 * as UTF-8 will display the "foreign" characters properly.  On Windows,
 * the preferred way to do that is by using UTF-16 (WCHAR, std::wstring),
 * but little (none?) of our existing code bases do that.  (On Linux, std::wstring
 * is typically UTF-32.)
 *
 */

namespace str
{
class EncodedStringView final
{
    std::string::const_pointer mpString = nullptr;
    sys::U8string::const_pointer mpU8String = nullptr;
    str::W1252string::const_pointer mpW1252String = nullptr;

public:
    EncodedStringView() = default;

    // Need these overload to avoid creating temporary std::basic_string<> instances.
    // Routnes always return a copy, never a reference, so there's no additional overhead
    // with storing a raw pointer rather than a pointer to  std::basic_string<>.
    EncodedStringView(sys::U8string::const_pointer);
    EncodedStringView(str::W1252string::const_pointer);
    explicit EncodedStringView(std::string::const_pointer);  // Assume platform native encoding: UTF-8 on Linux, Windows-1252 on Windows
    template<typename TChar>
    EncodedStringView(const std::basic_string<TChar>& s) : EncodedStringView(s.c_str())
    {
    }

    ~EncodedStringView() = default;
    EncodedStringView(const EncodedStringView&) = default;
    EncodedStringView& operator=(const EncodedStringView&) = default;
    EncodedStringView(EncodedStringView&&) = default;
    EncodedStringView& operator=(EncodedStringView&&) = default;

    //// std::string is encoded as specified on all platforms.
    //static EncodedStringView fromUtf8(const std::string&);
    //static EncodedStringView fromWindows1252(const std::string&);

    // Return a pointer to the viewed string, no conversion or copying.
    const char* c_str() const; // either mpString->c_str() or mpU8String->c_str() or mpW1252String->c_str()

    // Regardless of what string we're looking at, return a string in platform
    // native encoding: UTF-8 on Linux, Windows-1252 on Windows; this
    // might result in string conversion.
    std::string native() const; // c.f. std::filesystem::path::native()

    // Convert (perhaps) whatever we're looking at to UTF-8
    sys::U8string to_u8string() const;
    std::string& toUtf8(std::string&) const; // std::string is encoded as UTF-8, always.
};

}
#endif  // CODA_OSS_str_EncodedStringView_h_INCLLUDED_