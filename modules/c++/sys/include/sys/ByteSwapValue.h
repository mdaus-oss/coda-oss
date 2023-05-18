/* =========================================================================
 * This file is part of sys-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * (C) Copyright 2021, Maxar Technologies, Inc.
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

#ifndef CODA_OSS_sys_ByteSwapValue_h_INCLUDED_
#define CODA_OSS_sys_ByteSwapValue_h_INCLUDED_
#pragma once

#include <stdint.h>
#include <stdlib.h>
#ifdef __GNUC__
#include <byteswap.h>  // "These functions are GNU extensions."
#endif

#include <type_traits>
#include <coda_oss/span.h>
#include <coda_oss/cstddef.h>
#include <tuple>
#include <vector>
#include <array>

namespace sys
{
    // Overloads for common types
    inline constexpr uint8_t byteSwap(uint8_t val)
    {
        return val;  // no-op
    }
    #if defined(_MSC_VER)
    // These routines should generate a single instruction; see https://devblogs.microsoft.com/cppblog/a-tour-of-4-msvc-backend-improvements/
    inline uint16_t byteSwap(uint16_t val)
    {
        return _byteswap_ushort(val);
    }
    inline uint32_t byteSwap(uint32_t val)
    {
        return _byteswap_ulong(val);
    }
    inline uint64_t byteSwap(uint64_t val)
    {
        return _byteswap_uint64(val);
    }
    #elif defined(__GNUC__)
    inline uint16_t byteSwap(uint16_t val)
    {
        return bswap_16(val);
    }
    inline uint32_t byteSwap(uint32_t val)
    {
        return bswap_32(val);
    }
    inline uint64_t byteSwap(uint64_t val)
    {
        return bswap_64(val);
    }
    #endif

    namespace details
    {
    template <typename TUInt>
    inline auto swapIntBytes(const void* pIn_, void* pOut_)
    {
        static_assert(std::is_unsigned<TUInt>::value, "TUInt must be 'unsigned'");
        auto const pIn = static_cast<const TUInt*>(pIn_);
        auto const pOut = static_cast<TUInt*>(pOut_);
        *pOut = byteSwap(*pIn);

        // Give the raw byte-swapped bytes back to the caller for easy serialization
        return coda_oss::span<const coda_oss::byte>(static_cast<coda_oss::byte*>(pOut_), sizeof(TUInt));
    }

    template <size_t elemSize>
    inline auto swapBytes(const void* pIn, void* pOut)
    {
        auto const inPtr = static_cast<const coda_oss::byte*>(pIn);
        auto const outPtr = static_cast<coda_oss::byte*>(pOut);
        for (size_t ii = 0, jj = elemSize - 1; ii < jj; ++ii, --jj)
        {
            outPtr[ii] = inPtr[jj];
            outPtr[jj] = inPtr[ii];
        }

        // Give the raw byte-swapped bytes back to the caller for easy serialization
        return coda_oss::span<const coda_oss::byte>(outPtr, elemSize);
    }
    template <>
    inline auto swapBytes<sizeof(uint8_t)>(const void* pIn, void* pOut)
    {
        return swapIntBytes<uint8_t>(pIn, pOut);
    }
    template <>
    inline auto swapBytes<sizeof(uint16_t)>(const void* pIn, void* pOut)
    {
        return swapIntBytes<uint16_t>(pIn, pOut);
    }
    template <>
    inline auto swapBytes<sizeof(uint32_t)>(const void* pIn, void* pOut)
    {
        return swapIntBytes<uint32_t>(pIn, pOut);
    }
    template <>
    inline auto swapBytes<sizeof(uint64_t)>(const void* pIn, void* pOut)
    {
        return swapIntBytes<uint64_t>(pIn, pOut);
    }
    }

    /*!
    * Function to swap one element irrespective of size.
    * Returns the raw byte-swapped bytes for easy serialization.
    */
    template <typename T>
    inline coda_oss::span<const coda_oss::byte> swapBytes(const T* pIn, void* pOut)
    {
        // Byte-swapping a `struct` as a single object is unlikely to give the desired results;
        // instead, member should be individually byte-swapped.
        static_assert(!std::is_compound<T>::value, "T should not be a 'struct'");
        return details::swapBytes<sizeof(T)>(pIn, pOut);
    }
    template <typename T>
    inline auto swapBytes(T in, std::array<coda_oss::byte, sizeof(T)>& out)
    {
        return swapBytes(&in, out.data());
    }
    template <typename T>
    inline auto swapBytes(T in)
    {
        std::vector<coda_oss::byte> retval(sizeof(T));
        std::ignore = swapBytes(&in, retval.data());
        return retval;
    }

    /*!
     *  Function to swap one element irrespective of size.  The inplace
     *  buffer function should be preferred.
     *
     *  To specialize complex float, first include the complex library
     *  \code
        #include <complex>
     *  \endcode
     *
     *  Then put an overload in as specified below:
     *  \code
        template <typename T> std::complex<T> byteSwap(std::complex<T> val)
        {
            std::complex<T> out(byteSwap<T>(val.real()),
                                byteSwap<T>(val.imag()));
            return out;
        }
     *  \endcode
     *
     */
    template <typename T> inline T byteSwap(T val)
    {
        T out;
        std::ignore = swapBytes(&val, &out);
        return out;
    }
}
#endif // CODA_OSS_sys_ByteSwapValue_h_INCLUDED_
