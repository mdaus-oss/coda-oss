/* =========================================================================
 * This file is part of str-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * str-c++ is free software; you can redistribute it and/or modify
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


#pragma once
#ifndef CODA_OSS_str_Format_h_INCLUDED_
#define CODA_OSS_str_Format_h_INCLUDED_

#include <stdarg.h>
#include<stdint.h>
#include<stddef.h>

#include <string>

#include "config/Exports.h"

namespace str
{
    CODA_OSS_API std::string Format(const char* format, ...);
}

/*!
 *  \param format  The format
 *  \param ... Any printf like thing
 */
//CODA_OSS_API std::string format(const char* format, ...);

inline auto FmtX(const char* format)
{
    return str::Format(format);
}

inline auto FmtX(const char* format, const char* pStr)
{
    return str::Format(format, pStr);
}
inline auto FmtX(const char* format, const std::string& s)
{
    return FmtX(format, s.c_str());
}
inline auto FmtX(const char* format, int i)
{
    return str::Format(format, i);
}
inline auto FmtX(const char* format, uint32_t i)
{
    return str::Format(format, i);
}
inline auto FmtX(const char* format, ptrdiff_t l)
{
    return str::Format(format, l);
}
inline auto FmtX(const char* format, size_t ul)
{
    return str::Format(format, ul);
}
inline auto FmtX(const char* format, float f)
{
    return str::Format(format, f);
}
inline auto FmtX(const char* format, double d)
{
    return str::Format(format, d);
}

inline auto FmtX(const char* format, char ch, const char* pStr)
{
    return str::Format(format, ch, pStr);
}
inline auto FmtX(const char* format, char ch, const std::string& s)
{
    return FmtX(format, ch, s.c_str());
}
inline auto FmtX(const char* format, const std::string& s1, const std::string& s2)
{
    return str::Format(format, s1.c_str(), s2.c_str());
}
inline auto FmtX(const char* format, const std::string& s, size_t ul)
{
    return str::Format(format, s.c_str(), ul);
}
inline auto FmtX(const char* format, char ch1, char ch2)
{
    return str::Format(format, ch1, ch2);
}
inline auto FmtX(const char* format, long l1, long l2)
{
    return str::Format(format, l1, l2);
}
inline auto FmtX(const char* format, size_t ul1, size_t ul2)
{
    return str::Format(format, ul1, ul2);
}
inline auto FmtX(const char* format, double d1, double d2)
{
    return str::Format(format, d1, d2);
}
inline auto FmtX(const char* format, int i, const char* pStr)
{
    return str::Format(format, i, pStr);
}
inline auto FmtX(const char* fmt, int i, const std::string& s)
{
    return FmtX(fmt, i, s.c_str());
}

inline auto FmtX(const char* format, char ch1, char ch2, const std::string& s)
{
    return str::Format(format, ch1, ch2, s.c_str());
}
inline auto FmtX(const char* format, int i1, int i2, unsigned long ul)
{
    return str::Format(format, i1, i2, ul);
}
inline auto FmtX(const char* format, int i1, int i2, int i3)
{
    return str::Format(format, i1, i2, i3);
}
inline auto FmtX(const char* format, float f1, float f2, float f3)
{
    return str::Format(format, f1, f2, f3);
}
inline std::string FmtX(const char* format, const std::string& s1, int i2, int i3)
{
    return str::Format(format, s1.c_str(), i2, i3);
}
inline auto FmtX(const char* format, const std::string& s1, const std::string& s2, uint32_t ui)
{
    return str::Format(format, s1.c_str(), s2.c_str(), ui);
}
inline auto FmtX(const char* format, const std::string& s1, const std::string& s2, const std::string& s3)
{
    return str::Format(format, s1.c_str(), s2.c_str(), s3.c_str());
}

inline auto FmtX(const char* format, int i1, int i2, int i3, int i4)
{
    return str::Format(format, i1, i2, i3, i4);
}
inline auto FmtX(const char* format, const char* pStr1, const std::string& s2, const char* pStr3, const std::string& s4)
{
    return str::Format(format, pStr1, s2.c_str(), pStr3, s4.c_str());
}
inline auto FmtX(const char* format, const std::string& s1, int i2, const std::string& s3, int i4)
{
    return str::Format(format, s1.c_str(), i2, s3.c_str(), i4);
}

#endif  // CODA_OSS_str_Format_h_INCLUDED_
