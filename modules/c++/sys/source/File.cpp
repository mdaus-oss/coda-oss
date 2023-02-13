/* =========================================================================
 * This file is part of sys-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * (C) Copyright 2023, Maxar Technologies, Inc.
 *
 * sys-c++ is free software; you can redistribute it and/or modify
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

#include "sys/File.h"

#include "sys/Path.h"
#include "str/Manip.h"

sys::File sys::make_File(const coda_oss::filesystem::path& path, int accessFlags, int creationFlags)
{
    sys::File retval(std::nothrow, path, accessFlags, creationFlags);
    if (retval.isOpen())
    {
        return retval;
    }

    const auto checkIfExists = (creationFlags & sys::File::EXISTING) == sys::File::EXISTING;
    const auto expanded = sys::Path::expandEnvironmentVariables(path.string(), checkIfExists);
    return sys::File(expanded, accessFlags, creationFlags);
}

sys::File sys::make_File(const coda_oss::filesystem::path& parent, const coda_oss::filesystem::path& name,
        int accessFlags, int creationFlags)
{
    sys::File retval(std::nothrow, parent, name, accessFlags, creationFlags);
    if (retval.isOpen())
    {
        return retval;
    }

    const auto expanded_parent = sys::Path::expandEnvironmentVariables(parent.string(), coda_oss::filesystem::file_type::directory);
    // 'name' probably won't work without 'parent'
    const auto expanded_name =  sys::Path::expandEnvironmentVariables(name.string(), false /*checkIfExists*/);

    // let the File constructor deal with combining the expanded paths as well as checking for existence.
    return sys::File(expanded_parent, expanded_name, accessFlags, creationFlags);
}

#ifdef _WIN32
static FILE* fopen_(const std::string& fname, const std::string& mode)
{
    FILE* retval = nullptr;
    const auto result = fopen_s(&retval, fname.c_str(), mode.c_str());
    if (result != 0) // "Zero if successful; ..."
    {
        return nullptr;
    }
    return retval;
}
#else
static inline FILE* fopen_(const std::string& fname, const std::string& mode)
{
    return fopen(fname.c_str(), mode.c_str());
}
#endif

FILE* sys::fopen(const coda_oss::filesystem::path& fname, const std::string& mode)
{
    // Call  sys::expandEnvironmentVariables() if the initial fopen() fails.
    auto retval = fopen_(fname.string(), mode);
    if (retval != nullptr)
    {
        return retval;
    }

    constexpr bool checkIfExists = false; // TODO: parse "mode" to set checkIfExists?
    const auto expanded = sys::Path::expandEnvironmentVariables(fname.string(), checkIfExists);
    return fopen_(expanded, mode);
}
