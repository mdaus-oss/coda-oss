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
