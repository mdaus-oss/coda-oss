/* =========================================================================
 * This file is part of mdspan-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * (C) Copyright 2023, Maxar Technologies, Inc.
 *
 * mdspan-c++ is free software; you can redistribute it and/or modify
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
#ifndef CODA_OSS_mdspan_mdspan_h_INCLUDED_
#define CODA_OSS_mdspan_mdspan_h_INCLUDED_
#pragma once

#include <config/compiler_extensions.h>

CODA_OSS_disable_warning_push
#if _MSC_VER
//#pragma warning(disable: 4626) // '...' : assignment operator was implicitly defined as deleted
//#pragma warning(disable: 5027) // '...' : move assignment operator was implicitly defined as deleted
//#pragma warning(disable: 26487) // Don 't return a pointer '...' that may be invalid (lifetime.4).
//#pragma warning(disable: 4814) // '...': in C++14 '...' will not imply '...'; consider explicitly specifying '...'
#endif

#include <mdspan/mdspan>
#include <mdspan/mdarray>

CODA_OSS_disable_warning_pop


#endif  // CODA_OSS_mdspan_mdspan_h_INCLUDED_
