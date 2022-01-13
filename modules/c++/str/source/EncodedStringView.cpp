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
#include <string.h>

#include <stdexcept>

#include "str/Convert.h"
#include "str/Encoding.h"

str::EncodedStringView::EncodedStringView(std::string::const_pointer s) : mString(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}
str::EncodedStringView::EncodedStringView(sys::U8string::const_pointer s) : mU8String(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}
str::EncodedStringView::EncodedStringView(str::W1252string::const_pointer s) : mW1252String(s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
}
str::EncodedStringView::EncodedStringView(const std::string& s) : mString(s)
{
}
str::EncodedStringView::EncodedStringView(const sys::U8string& s) : mU8String(s)
{
}
str::EncodedStringView::EncodedStringView(const str::W1252string& s) : mW1252String(s)
{
}

str::EncodedStringView& str::EncodedStringView::operator=(std::string::const_pointer s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
    mString = s;
    mU8String.clear();
    mW1252String.clear();
    return *this;
}
str::EncodedStringView& str::EncodedStringView::operator=(const std::string& s)
{
    mString = s;
    mU8String.clear();
    mW1252String.clear();
    return *this;
}
str::EncodedStringView& str::EncodedStringView::operator=(sys::U8string::const_pointer s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }

    mU8String = s;
    mString.clear();
    mW1252String.clear();
    return *this;
}
str::EncodedStringView& str::EncodedStringView::operator=(const sys::U8string& s)
{
    mU8String = s;
    mString.clear();
    mW1252String.clear();
    return *this;
}
str::EncodedStringView& str::EncodedStringView::operator=(str::W1252string::const_pointer s)
{
    if (s == nullptr)
    {
        throw std::invalid_argument("s is NULL.");
    }
    mW1252String = s;
    mString.clear();
    mU8String.clear();
    return *this;
}
str::EncodedStringView& str::EncodedStringView::operator=(const str::W1252string& s)
{
    mW1252String = s;
    mString.clear();
    mU8String.clear();
    return *this;
}

inline const char* c_str(
    std::string::const_pointer mpString,
    sys::U8string::const_pointer mpU8String,
    str::W1252string::const_pointer mpW1252String)
{
    // Be sure we can cast between the different types
    static_assert(sizeof(*mpString) == sizeof(*mpU8String), "wrong string sizes");
    static_assert(sizeof(*mpString) == sizeof(*mpW1252String), "wrong string sizes");
    static_assert(sizeof(*mpU8String) == sizeof(*mpW1252String), "wrong string sizes");

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

    throw std::logic_error("Can't determine c_str() result.");
}

std::string str::EncodedStringView::native() const
{
    if (!mString.empty())
    {
        assert(mU8String.empty());
        assert(mW1252String.empty());
        return mString.c_str(); // easy-peasy
    }

    if (!mU8String.empty())
    {
        assert(mString.empty());
        assert(mW1252String.empty());

        std::string retval;
        str::details::toString(mU8String.c_str(), retval);
        return retval;
    }

    // This internal helper routine will convert from Windows-1252 to UTF-8 on Linux
    if (!mW1252String.empty())
    {
        assert(mString.empty());
        assert(mU8String.empty());

        std::string retval;
        str::details::toNative(mW1252String.c_str(), retval);
        return retval;
    }

    throw std::logic_error("Can't determine native() result");
}

sys::U8string str::EncodedStringView::to_u8string() const
{
    const auto sz = strlen(::c_str(mString.c_str(), mU8String.c_str(), mW1252String.c_str()));
    if (!mString.empty())
    {
        return str::to_u8string(mString.c_str(), sz);
    }
    if (!mU8String.empty())
    {
        return str::to_u8string(mU8String.c_str(), sz);
    }
    if (!mW1252String.empty())
    {
        return str::to_u8string(mW1252String.c_str(), sz);
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

bool str::EncodedStringView::operator_eq(const EncodedStringView& rhs) const
{
    auto& lhs = *this;

    // If all the pointers are all the same, the views must be equal
    if ((lhs.mString.c_str() == rhs.mString.c_str())
        && (lhs.mU8String.c_str() == rhs.mU8String.c_str())
        && (lhs.mW1252String.c_str() == rhs.mW1252String.c_str()))
    {
        assert(! ((lhs.mString.c_str() == nullptr) && (lhs.mU8String.c_str() == nullptr) && (lhs.mW1252String.c_str() == nullptr)) );
        return true;
    }
    
    if (!lhs.mString.empty() && !rhs.mString.empty())
    {
        assert(lhs.mU8String.empty() && rhs.mU8String.empty());
        assert(lhs.mW1252String.empty() && rhs.mW1252String.empty());
        return strcmp(lhs.mString.c_str(), rhs.mString.c_str()) == 0;
    }
    if (!lhs.mU8String.empty() && !rhs.mU8String.empty())
    {
        assert(lhs.mString.empty() && rhs.mString.empty());
        assert(lhs.mW1252String.empty() && rhs.mW1252String.empty());
        return strcmp(str::cast<const char*>(lhs.mU8String.c_str()), str::cast<const char*>(rhs.mU8String.c_str())) == 0;
    }
    if (!lhs.mW1252String.empty() && !rhs.mW1252String.empty())
    {
        assert(lhs.mString.empty() && rhs.mString.empty());
        assert(lhs.mU8String.empty() && rhs.mU8String.empty());
        return strcmp(str::cast<const char*>(lhs.mW1252String.c_str()), str::cast<const char*>(rhs.mW1252String.c_str())) == 0;
    }

    // LHS and RHS have different encodings
    if (!rhs.mU8String.empty()) // prefer UTF-8
    {
        // We KNOW lhs.mpU8String is NULL because of check above
        assert(lhs.mU8String.empty()); // should have used strcmp(), aboe
        return lhs.to_u8string() == rhs.mU8String.c_str();
    }
    if (!rhs.mString.empty()) // not UTF-8, try native
    {
        // We KNOW lhs.mpString is NULL because of check above
        assert(lhs.mString.empty());  // should have used strcmp(), aboe
        return lhs.native() == rhs.mString.c_str();
    }

    // One side (but not both) must be Windows-1252; convert to UTF-8 for comparison
    return lhs.to_u8string() == rhs.to_u8string();
}
