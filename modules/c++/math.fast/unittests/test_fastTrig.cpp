/* =========================================================================
 * This file is part of math.fast-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2017, MDA Information Systems LLC
 * � Copyright 2023, Maxar Technologies, Inc. 
 *
 * math.fast-c++ is free software; you can redistribute it and/or modify
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
#include <std/numbers>
#include <chrono>
#include <std/span>

#include <coda_oss/CPlusPlus.h>
#include <math/fast/Trig.h>
#include <sys/Span.h>
#include <sys/Dbg.h>
#include <sys/OS.h>

#include "TestCase.h"

template<typename T>
static std::vector<T> make_values(size_t iterations)
{
    constexpr auto pi = std::numbers::pi;

    // https://en.cppreference.com/w/cpp/numeric/math/sin
    static const std::vector<T> angles{static_cast<T>(pi / 6), static_cast<T>(pi / 2), static_cast<T>(-3 * pi / 4),
        static_cast<T>(0.0), static_cast<T>(-0.0)};

    std::vector<T> retval;
    retval.reserve(angles.size() * iterations);
    for (size_t i = 0; i<iterations; i++)
    {
        retval.insert(retval.end(), angles.begin(), angles.end());
    }
    return retval;
}

template<typename T>
static void slow_Sin(coda_oss::span<const T> inputs, coda_oss::span<T> outputs)
{
    // This is intentionally slow so that we have something with which to compare.
    // Note that no effort is made to **prevent** auto-vectorization by the compiler.
    auto begin = inputs.begin();
    const auto end = inputs.end();
    auto it = outputs.begin();
    while (begin != end)
    {
        *it++ = sin(*begin++);
    }
}

static void test_simd_Sin_almost_equal(const std::string& testName, const float* pResults, const size_t size = 5)
{
    if (size >= 1) TEST_ASSERT_ALMOST_EQ(pResults[0], 0.5f);
    if (size >= 2) TEST_ASSERT_ALMOST_EQ(pResults[1], 1.0f);
    if (size >= 3) TEST_ASSERT_ALMOST_EQ(pResults[2], -0.7071067812f);
    if (size >= 4) TEST_ASSERT_ALMOST_EQ(pResults[3], 0.0f);
    if (size >= 5) TEST_ASSERT_ALMOST_EQ(pResults[4], -0.0f);
}
static void test_Sin(const std::string& testName, math::fast::execution_policy policy,
    double expected_ratio)
{
    constexpr size_t iterations = sys::release ? 4000000 : 400;

    const auto inputs_ = make_values<float>(iterations);
    const auto inputs = sys::make_span(inputs_);
    std::vector<float> results_(inputs.size());
    const auto results = sys::make_span(results_);

    math::fast::Sin(policy, inputs, results);
    auto start = std::chrono::steady_clock::now();
    math::fast::Sin(policy, inputs, results);
    auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_fast = end - start;

    test_simd_Sin_almost_equal(testName, &(results[0]));
    
    slow_Sin(inputs, results);
    start = std::chrono::steady_clock::now();
    slow_Sin(inputs, results);
    end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_slow = end - start;

    test_simd_Sin_almost_equal(testName, &(results[0]));

    const auto ratio = elapsed_slow / elapsed_fast;
    if constexpr (sys::release) // DEBUG code is slow
    {
        TEST_ASSERT(ratio >= expected_ratio);
    }
    else
    {
        TEST_ASSERT(ratio > 0.0);    
    }
}
TEST_CASE(Test_Sin)
{
    // Ratios observed by testing
    // constexpr auto expected_ratio = sys::Platform == sys::PlatformType::Windows ? 2.5 : 2.25;
    test_Sin(testName, math::fast::execution_policy::par_unseq, 8.0);

    test_Sin(testName, math::fast::execution_policy::par, 8.0);

    #if CODA_OSS_cpp20
    test_Sin(testName, math::fast::execution_policy::unseq, 9.0);
    #endif
}

TEST_MAIN(

    TEST_CHECK(Test_Sin);
)
