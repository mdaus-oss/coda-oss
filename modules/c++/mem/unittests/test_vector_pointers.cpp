/* =========================================================================
 * This file is part of mem-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * mem-c++ is free software; you can redistribute it and/or modify
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

#include <vector>
#include <complex>
#include <std/span>

#include <mem/VectorOfPointers.h>

#include "TestCase.h"

TEST_CASE(testVecOfRawPointers)
{
    mem::VectorOfPointers<int> myVec;

    myVec.push_back(new int(1));
    myVec.push_back(new int(2));
    myVec.push_back(new int(3));
    myVec.push_back(new int(4));
    myVec.push_back(new int(5));

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(5));

    myVec.erase(myVec.begin() + 2);

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(4));
    TEST_ASSERT_EQ(*myVec[2], 4);

    myVec.push_back(new int(6));

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(5));

    myVec.erase(myVec.begin(), myVec.begin() + 4);

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(1));
    TEST_ASSERT_EQ(*myVec[0], 6);

    myVec.erase(myVec.begin());
    TEST_ASSERT_TRUE(myVec.empty());
}

static mem::VectorOfSharedPointers<int> mem_VectorOfSharedPointers_int()
{
    mem::VectorOfSharedPointers<int> retval;
    retval.push_back(new int(1));
    return retval;
}

static std::vector<std::shared_ptr<int>> std_vector_shared_ptr_int()
{
    std::vector<std::shared_ptr<int>> retval;
    retval.push_back(std::make_shared<int>(1));
    return retval;
}

TEST_CASE(testVecOfSharedPointers)
{
    mem::VectorOfSharedPointers<int> myVec;

    myVec.push_back(new int(1));
    myVec.push_back(new int(2));
    myVec.push_back(new int(3));
    myVec.push_back(new int(4));
    myVec.push_back(new int(5));

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(5));

    myVec.erase(myVec.begin() + 2);

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(4));
    TEST_ASSERT_EQ(*myVec[2], 4);

    myVec.push_back(new int(6));

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(5));

    myVec.erase(myVec.begin(), myVec.begin() + 4);

    TEST_ASSERT_EQ(myVec.size(), static_cast<size_t>(1));
    TEST_ASSERT_EQ(*myVec[0], 6);

    myVec.erase(myVec.begin());
    TEST_ASSERT_TRUE(myVec.empty());

    {
        mem::VectorOfSharedPointers<int> myVec2 = mem_VectorOfSharedPointers_int();  // copy
        myVec = mem_VectorOfSharedPointers_int();  // assignment
    }
    {
        mem::VectorOfSharedPointers<int> myVec2 = std_vector_shared_ptr_int();  // copy
        myVec = std_vector_shared_ptr_int();  // assignment
    }

    {
       std::vector<std::shared_ptr<int>> myVec2 = mem_VectorOfSharedPointers_int();  // copy
    }
    {
       std::vector<std::shared_ptr<int>> myVec2 = std_vector_shared_ptr_int();  // copy
    }
}

TEST_CASE(testSpanCxFloat)
{
    const std::vector<std::complex<float>> data{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    const std::span<const std::complex<float>> view(data.data(), data.size());

    TEST_ASSERT_EQ(data.size(), view.size());

    TEST_ASSERT_EQ(view[0].real(), 1);
    TEST_ASSERT_EQ(view[0].imag(), 2);
    TEST_ASSERT_EQ(view[1].real(), 3);
    TEST_ASSERT_EQ(view[1].imag(), 4);
    TEST_ASSERT_EQ(view[2].real(), 5);
    TEST_ASSERT_EQ(view[2].imag(), 6);
    TEST_ASSERT_EQ(view[3].real(), 7);
    TEST_ASSERT_EQ(view[3].imag(), 8);
}

TEST_MAIN(
    TEST_CHECK(testVecOfRawPointers);
    TEST_CHECK(testVecOfSharedPointers);

    TEST_CHECK(testSpanCxFloat);
    )
