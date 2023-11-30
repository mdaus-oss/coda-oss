/* =========================================================================
 * This file is part of coda_oss-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * � Copyright 2023, Maxar Technologies, Inc.
 *
 * coda_oss-c++ is free software; you can redistribute it and/or modify
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

#include <assert.h>

#include <array>

#include "coda_oss/span.h"

// This is a super-simple partial and incomplete implementation of `std::mdspan` (in C++23).
// It's just enough to allow *hdf5::lite::SpanRC* to work with ours or a more complete one.
// https://en.cppreference.com/w/cpp/container/mdspan
namespace coda_oss
{
namespace details
{
template<typename IndexType, size_t Rank>
struct dextents final
{
    static_assert(Rank == 2, "Rank must have a value of 2");
    using index_type = IndexType;
    using size_type = index_type;
    using rank_type = size_t;

    constexpr dextents() = default;
    constexpr dextents(index_type i0, index_type i1) noexcept : exts_{i0, i1}
    {
    }
    constexpr explicit dextents(const std::array<index_type, Rank>& exts) noexcept : exts_(exts)
    {
    }
    dextents(const dextents&) = default;
    dextents& operator=(const dextents&) = default;
    dextents(dextents&&) = default;
    dextents& operator=(dextents&&) = default;

    constexpr index_type extent(rank_type r) const noexcept
    {
        return exts_[r];
    }

    static constexpr auto rank() noexcept
    {
        return Rank;
    }

private:
    std::array<index_type, Rank> exts_;
};

template <typename IndexType, size_t Rank>
inline size_t area(const dextents <IndexType, Rank>& exts)
{
    return exts.extent(0) * exts.extent(1);
}

template<typename T, typename TExtents>
class mdspan final
{
    coda_oss::span<T> s_;
    TExtents ext_;

public:
    using extents_type = TExtents;
    using size_type = typename extents_type::size_type;
    using data_handle_type = T*;
    using reference = T&;

    constexpr mdspan() = default;
    mdspan(data_handle_type p, const extents_type& ext) noexcept : s_(p, area(ext)), ext_(ext)
    {
    }
    mdspan(data_handle_type p, const std::array<size_type, 2>& dims) noexcept : mdspan(p, extents_type(dims))
    {
    }
    mdspan(const mdspan&) = default;
    mdspan& operator=(const mdspan&) = default;
    mdspan(mdspan&&) = default;
    mdspan& operator=(mdspan&&) = default;

    constexpr data_handle_type data_handle() const noexcept
    {
        return s_.data();
    }

    /*constexpr*/ reference operator[](size_t idx) const noexcept
    {
        assert(idx < size());  // prevents "constexpr" in C++11
        return data_handle()[idx];
    }
    /*constexpr*/ reference operator()(size_t r, size_t c) const noexcept
    {
        const auto offset = (r * extent(1)) + c;
        return (*this)[offset];
    }

    constexpr size_t size() const noexcept
    {
        return s_.size();
    }

    constexpr bool empty() const noexcept
    {
        return s_.empty();
    }
    
    auto extent(size_type rank) const
    {
        return ext_.extent(rank);
    }

    static constexpr auto rank() noexcept
    {
        return 2;
    }
};

}

}

