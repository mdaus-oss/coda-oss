/* =========================================================================
 * This file is part of xml.lite-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 * � Copyright 2023, Maxar Technologies, Inc.
 *
 * xml.lite-c++ is free software; you can redistribute it and/or modify
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

/*!
 * \file  DOMAttr.h
 * \brief File describing the tree API
 */

#include <memory>
#include <coda_oss/string.h>

#include <config/Exports.h>

#include "QName.h"

namespace xml
{
namespace lite
{
struct AttributeNode;

/*!
 * \class DOMAttr
 * \brief Wrapper around AttributeNode that tries to follow
 * https://xerces.apache.org/xerces-c/ApacheDOMC++Binding.html
 */
class CODA_OSS_API DOMAttr final
{
    AttributeNode* pAttributeNode = nullptr;

public:
    DOMAttr() = default;
    DOMAttr(AttributeNode&);
    ~DOMAttr() = default;

    DOMAttr(const DOMAttr&) = delete;
    DOMAttr& operator=(const DOMAttr&) = delete;
    DOMAttr(DOMAttr&&) = default;
    DOMAttr& operator=(DOMAttr&&) = default;

    /*!
     *  See DOMNode.hpp
     */
    Uri getNamespaceURI() const;
    std::string getPrefix() const;
    std::string getLocalName() const;


    // https://xerces.apache.org/xerces-c/apiDocs-3/classDOMAttr.html#adacdb9c2e42946077b481da116a36527
    std::string getName() const;

    // https://xerces.apache.org/xerces-c/apiDocs-3/classDOMAttr.html#a8c2f7efb05dcd48118a6d3f990dff589
    coda_oss::u8string getValue() const;
    // https://xerces.apache.org/xerces-c/apiDocs-3/classDOMAttr.html#ad2660958e94797d918114df04adb87ff
    void setValue(const coda_oss::u8string&);
};

}
}


