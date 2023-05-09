/* =========================================================================
 * This file is part of sys-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2017, MDA Information Systems LLC
 *
 * sys-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

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

#include "TestCase.h"

#include <stdint.h>

#include <array>
#include <std/bit> // std::endian
#include <std/cstddef> // std::byte

#include <sys/Conf.h>

TEST_CASE(testEndianness)
{
    /*const*/ auto native = std::endian::native; // "const" causes "conditional expression is constant."

    if (native == std::endian::big) { }
    else if (native == std::endian::little) { }
    else
    {
        TEST_FAIL_MSG("Mixed-endian not supported!");
    }

    const bool isBigEndianSystem = sys::isBigEndianSystem();

    if (native == std::endian::big)
    {
        TEST_ASSERT(isBigEndianSystem);
    }
    else
    {
        TEST_ASSERT(!isBigEndianSystem);    
    }
    if (native == std::endian::little)
    {
        TEST_ASSERT(!isBigEndianSystem);
    }
    else
    {
        TEST_ASSERT(isBigEndianSystem);
    }


    if (isBigEndianSystem)
    {
        TEST_ASSERT(native == std::endian::big);
    }
    else
    {
        TEST_ASSERT(native == std::endian::little);    
    }
}

TEST_CASE(testByteSwap)
{
    ::srand(334);

    static const size_t NUM_PIXELS = 10000;
    std::vector<sys::Uint64_T> origValues(NUM_PIXELS);
    for (size_t ii = 0; ii < NUM_PIXELS; ++ii)
    {
        const auto value = static_cast<float>(::rand()) / RAND_MAX *
                std::numeric_limits<sys::Uint64_T>::max();
        origValues[ii] = static_cast<sys::Uint64_T>(value);
    }

    // Byte swap the old-fashioned way
    std::vector<sys::Uint64_T> values1(origValues);
    sys::byteSwap(&values1[0], sizeof(sys::Uint64_T), NUM_PIXELS);


    // Byte swap into output buffer
    const std::vector<sys::Uint64_T> values2(origValues);
    std::vector<sys::Uint64_T> swappedValues2(values2.size());
    sys::byteSwap(&values2[0], sizeof(sys::Uint64_T), NUM_PIXELS,
                  &swappedValues2[0]);

    // Everything should match
    for (size_t ii = 0; ii < NUM_PIXELS; ++ii)
    {
        TEST_ASSERT_EQ(values1[ii], swappedValues2[ii]);
    }
}

TEST_CASE(testByteSwapInts)
{
    //const std::byte one_byte{ static_cast<std::byte>(0x31) };
    //const auto swap1 = sys::byteSwap(one_byte);
    //TEST_ASSERT(one_byte == swap1);

    constexpr std::byte two_bytes[] { static_cast<std::byte>(0x31), static_cast<std::byte>(0x41) };
    const void* pBytes = &(two_bytes[0]);
    const auto pInt16 = static_cast<const int16_t*>(pBytes);
    const auto swap2 = sys::byteSwap(*pInt16);
    const void* pResult_ = &swap2;
    auto pResult = static_cast<const std::byte*>(pResult_);
    TEST_ASSERT(pResult[0] == two_bytes[1]);
    TEST_ASSERT(pResult[1] == two_bytes[0]);

    constexpr std::byte four_bytes[] { static_cast<std::byte>(0x31), static_cast<std::byte>(0x41), static_cast<std::byte>(0x59), static_cast<std::byte>(0x26)};
    pBytes = &(four_bytes[0]);
    const auto pInt32 = static_cast<const int32_t*>(pBytes);
    const auto swap4 = sys::byteSwap(*pInt32);
    pResult_ = &swap4;
    pResult = static_cast<const std::byte*>(pResult_);
    TEST_ASSERT(pResult[0] == four_bytes[3]);
    TEST_ASSERT(pResult[1] == four_bytes[2]);
    TEST_ASSERT(pResult[2] == four_bytes[1]);
    TEST_ASSERT(pResult[3] == four_bytes[0]);

    constexpr std::byte eight_bytes[] { static_cast<std::byte>(0x31), static_cast<std::byte>(0x41), static_cast<std::byte>(0x59), static_cast<std::byte>(0x26),
     static_cast<std::byte>(0x01), static_cast<std::byte>(0x23), static_cast<std::byte>(0x45), static_cast<std::byte>(0x67)};
    pBytes = &(eight_bytes[0]);
    const auto pInt64 = static_cast<const int64_t*>(pBytes);
    const auto swap8 = sys::byteSwap(*pInt64);
    pResult_ = &swap8;
    pResult = static_cast<const std::byte*>(pResult_);
    TEST_ASSERT(pResult[0] == eight_bytes[7]);
    TEST_ASSERT(pResult[1] == eight_bytes[6]);
    TEST_ASSERT(pResult[2] == eight_bytes[5]);
    TEST_ASSERT(pResult[3] == eight_bytes[4]);
    TEST_ASSERT(pResult[4] == eight_bytes[3]);
    TEST_ASSERT(pResult[5] == eight_bytes[2]);
    TEST_ASSERT(pResult[6] == eight_bytes[1]);
    TEST_ASSERT(pResult[7] == eight_bytes[0]);
}


TEST_MAIN(
    TEST_CHECK(testEndianness);
    TEST_CHECK(testByteSwap);
    TEST_CHECK(testByteSwapInts);
    )
     