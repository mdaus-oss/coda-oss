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
#include "str/Manip.h"

#include <limits.h>
#include <stdio.h>
#include <wctype.h>
#include <assert.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "str/Convert.h"
#include "str/Encoding.h"

namespace
{
char transformCheck(int c, int (*transform)(int))
{
    // Ensure the character can be represented
    // as an unsigned char or is 'EOF', as the
    // behavior for all other characters is undefined
    if ((c >= 0 && c <= UCHAR_MAX) || c == EOF)
    {
        return static_cast<char>(transform(c));
    }
    else
    {
        // Invalid char for transform: no-op
        return static_cast<char>(c);
    }
}

char tolowerCheck(char c)
{
    return transformCheck(c, tolower);
}

char toupperCheck(char c)
{
    return transformCheck(c, toupper);
}
}


namespace str
{
void trim(std::string & s)
{
    size_t i;
    for (i = 0; i < s.length(); i++)
    {
        if (!iswspace(s[i]))
            break;
    }
    s.erase(0, i);

    for (i = s.length() - 1; (int) i >= 0; i--)
    {
        if (!iswspace(s[i]))
            break;

    }
    if (i + 1 < s.length())
        s.erase(i + 1);
}

bool endsWith(const std::string & s, const std::string & match)
{
    const size_t mLen = match.length();
    const size_t sLen = s.length();
    for (size_t i = 0; i < sLen && i < mLen; ++i)
        if (!(s[sLen - i - 1] == match[mLen - i - 1]))
            return false;
    return sLen >= mLen;
}

bool startsWith(const std::string & s, const std::string & match)
{
    const size_t mLen = match.length();
    const size_t sLen = s.length();
    for (size_t i = 0; i < sLen && i < mLen; ++i)
        if (!(s[i] == match[i]))
            return false;
    return sLen >= mLen;
}

size_t replace(std::string& str,
               const std::string& search,
               const std::string& replace,
               size_t start)
{
    size_t index = str.find(search, start);

    if (index != std::string::npos)
    {
        str.replace(index, search.length(), replace);
        start = index;
    }
    else
    {
        start = str.length();
    }

    return start;        
}

void replaceAll(std::string& string,
                const std::string& search,
                const std::string& replace)
{
    size_t start = 0;
    while (start < string.length())
    {
        start = str::replace(string, search, replace, start);
        // skip ahead --
        // avoids inifinite loop if replace contains search 
        start += replace.length();                             
    }
}

bool contains(const std::string& str, const std::string& match)
{
    return str.find(match) != std::string::npos;
}

inline bool isTest(const std::string& s, int (*is)(int))
{
    for (const auto& ch : s)
    {
        if (!is(ch))
            return false;
    }
    return !s.empty();
}

bool isAlpha(const std::string& s)
{
    return isTest(s, isalpha);
}

template<typename Pred>
inline bool isTest(const std::string& s, int (*is1)(int), Pred is2)
{
    for (const auto& ch : s)
    {
        if (!is1(ch) && !is2(ch))
            return false;
    }
    return !s.empty();
}

bool isAlphaSpace(const std::string& s)
{
    return isTest(s, isalpha, isspace);
}

bool isNumeric(const std::string& s)
{
    return isTest(s, isdigit);
}

bool isNumericSpace(const std::string& s)
{
    return isTest(s, isdigit, isspace);
}

bool isWhitespace(const std::string& s)
{
    for (const auto& ch : s)
    {
        if (!isspace(ch))
            return false;
    }
    return true;
}

bool isAlphanumeric(const std::string& s)
{
    return isTest(s, isalpha, isdigit);
}

bool isAsciiPrintable(const std::string& s)
{
    for (const auto& ch : s)
    {
        char c = ch;
        if (c < 32 || c > 126)
            return false;
    }
    return true;
}

bool containsOnly(const std::string& s, const std::string& validChars)
{
    typedef std::string::const_iterator StringIter;
    std::vector<bool> chars(255, false);
    for (StringIter it = validChars.begin(); it != validChars.end(); ++it)
        chars[(unsigned int)*it] = true;
    for (StringIter it = s.begin(); it != s.end(); ++it)
        if (!chars[(unsigned int)*it])
            return false;
    return true;
}

std::vector<std::string> split(const std::string& s,
        const std::string& splitter, size_t maxSplit)
{
    std::vector < std::string > vec;
    const auto str_l = s.length();
    const auto split_l = splitter.length();
    size_t pos = 0;
    while (pos < str_l && maxSplit != 1)
    {
        auto nextPos = s.find(splitter, pos);
        if (nextPos == std::string::npos)
            nextPos = str_l;
        if (nextPos != pos)
            vec.push_back(s.substr(pos, nextPos - pos));
        pos = nextPos + split_l;
        if (maxSplit != std::string::npos && vec.size() >= maxSplit - 1)
            break;
    }

    if (pos < str_l)
        vec.push_back(s.substr(pos));

    return vec;
}

static std::string fromWString(const std::wstring& s)
{
    std::unique_ptr<sys::U8string> u8;
    std::unique_ptr<str::W1252string> w1252;

    if (!str::fromWString(s, u8, w1252))
    {
        throw std::runtime_error("wcsrtombs() failed.");    
    }

    if (u8)
    {
        assert(!w1252);
        return str::cast<std::string::const_pointer>(u8->c_str());
    }
    if (w1252)
    {
        assert(!u8);
        return str::cast<std::string::const_pointer>(w1252->c_str());
    }

    throw std::runtime_error("fromWString() failed.");    
}

template <typename TChar, typename Fn>
inline void transform(std::basic_string<TChar>& s, Fn f)
{
    (void) std::transform(s.begin(), s.end(), s.begin(), f);
}

static std::string to___er(const std::string& s, int (*to)(int), void (*tow)(std::wstring&)) // "___" = "low" or "upp"
{
    // tolowerCheck() on std::string doesn't work for "special" characters like '�' (used by French-speaking Canadians).
    auto platform = details::Platform; // "conditional expression is constant"
    if (platform == details::PlatformType::Windows)
    {    
        // On Windows, assume Windows-1252. tolower() works, but NOT our own tolowerCheck()
        const setlocale locale;
        auto retval = s;
        transform(retval, [&](int ch) {
                           return static_cast<std::string::value_type>(to(ch));
                       });
        return retval;
    }
    else
    {
        // Not Windows, assume string is UTF-8
        auto w_s = to_wstring(s);
        tow(w_s);
        return fromWString(w_s);
    }
}

void lower(std::string& s)
{
    transform(s, tolowerCheck);
}
void lower(std::wstring& s)
{
    // Need a UTF-8 locale (or something other than "C") for this to work
    // https://en.cppreference.com/w/c/string/wide/towlower
    const setlocale locale;
    transform(s, towlower);
}
std::string toLower(const std::string& s)
{
   return to___er(s, tolower, lower);
}

void upper(std::string& s)
{
    transform(s, toupperCheck);
}
void upper(std::wstring& s)
{
    // Need a UTF-8 locale (or something other than "C") for this to work
    // https://en.cppreference.com/w/c/string/wide/towlower
    const setlocale locale;
    transform(s, towupper);
}
std::string toUpper(const std::string& s)
{
    return to___er(s, toupper, upper);
}

void escapeForXML(std::string& str)
{
    // & needs to be first or else it'll mess up the other characters that we
    // replace
    replaceAll(str, "&", "&amp;");
    replaceAll(str, "<", "&lt;");
    replaceAll(str, ">", "&gt;");
    replaceAll(str, "\"", "&quot;");
    replaceAll(str, "'", "&apos;");
    replaceAll(str, "\n", "&#10;");
    replaceAll(str, "\r", "&#13;");
}
}
