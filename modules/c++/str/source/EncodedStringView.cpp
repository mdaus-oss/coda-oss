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

#include "str/EncodedStringView.h"

#include <assert.h>

#include <stdexcept>

#include "str/Convert.h"
#include "str/Encoding.h"

str::EncodedStringView::EncodedStringView(std::string::const_pointer s) : mpString(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}
str::EncodedStringView::EncodedStringView(sys::U8string::const_pointer s) : mpU8String(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}
str::EncodedStringView::EncodedStringView(str::W1252string::const_pointer s) : mpW1252String(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}

const char* str::EncodedStringView::c_str() const
{
    if (mpString != nullptr)
    {
        assert(mpU8String == nullptr);
        assert(mpW1252String == nullptr);
        return mpString;
    }
    if (mpU8String != nullptr)
    {
        assert(mpString == nullptr);
        assert(mpW1252String == nullptr);
        return str::cast<const char*>(mpU8String);
    }
    if (mpW1252String != nullptr)
    {
        assert(mpString == nullptr);
        assert(mpU8String == nullptr);
        return str::cast<const char*>(mpW1252String);
    }

    throw std::logic_error("Can't determine c_str() result");
}

std::string str::EncodedStringView::native() const
{
    if (mpString != nullptr)
    {
        assert(mpU8String == nullptr);
        assert(mpW1252String == nullptr);
        return mpString; // easy-peasy
    }

    // For a UTF-8 string, str::toString() will convert; see the specialization in Encoding.cpp
    if (mpU8String != nullptr)
    {
        assert(mpString == nullptr);
        assert(mpW1252String == nullptr);

        std::string retval;
        str::details::toString(mpU8String, retval);
        return retval;
    }

    // This internal helper routine will convert from Windows-1252 to UTF-8 on Linux
    if (mpW1252String != nullptr)
    {
        assert(mpString == nullptr);
        assert(mpU8String == nullptr);

        std::string retval;
        str::details::toNative(mpW1252String, retval);
        return retval;
    }

    throw std::logic_error("Can't determine native() result");
}

sys::U8string str::EncodedStringView::to_u8string() const
{
    const auto sz = strlen(c_str());
    if (mpString != nullptr)
    {
        return str::to_u8string(mpString, sz);
    }
    if (mpU8String != nullptr)
    {
        return str::to_u8string(mpU8String, sz);
    }
    else if (mpW1252String != nullptr)
    {
        return str::to_u8string(mpW1252String, sz);
    }
    
    throw std::logic_error("Can't determine to_u8string() result");
}
std::string& str::EncodedStringView::toUtf8(std::string& result) const
{
    // This is easy, but creates "unneeded" sys::U8string; it would be
    // better to put the result directly into std::string
    const auto utf8 = to_u8string(); // TODO: avoid this copy
    result = str::c_str<std::string::const_pointer>(utf8);
    return result;
}