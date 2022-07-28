/* =========================================================================
 * This file is part of io-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * io-c++ is free software; you can redistribute it and/or modify
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

#include <std/string>
#include "coda_oss/CPlusPlus.h"
#include "io/StringStream.h"
#include <TestCase.h>

#include "xml/lite/MinidomParser.h"

// It seems that a macro is better than a utility routine, see https://github.com/tahonermann/char8_t-remediation
// C++20 changed the type of u8 to char8_t* https://en.cppreference.com/w/cpp/language/string_literal
// Not putting this everywhere because (1) well, it's a macro, and (2) it's mostly
// only test code that uses string literals.
#if CODA_OSS_cpp20
#define U8(s) u8##s
#else
#define U8(s) static_cast<const std::char8_t*>(static_cast<const void*>(s))
#endif

static inline xml::lite::StringEncoding getEncoding(const xml::lite::Element& element)
{
    return element.getEncoding();
}
static void test_assert_eq(const std::string& testName,
    xml::lite::StringEncoding lhs, xml::lite::StringEncoding rhs)
{
    TEST_ASSERT(lhs == rhs);
}
static void test_assert_eq(const std::string& testName,
    const xml::lite::Element& lhs, xml::lite::StringEncoding rhs)
{
    test_assert_eq(testName, getEncoding(lhs), rhs);
}
static void test_assert_utf8(const std::string& testName, const xml::lite::Element& e)
{
    test_assert_eq(testName, e, xml::lite::StringEncoding::Utf8);
}
static void test_assert_platform(const std::string& testName, const xml::lite::Element& e)
{
    test_assert_eq(testName, e, xml::lite::PlatformEncoding);
}

static const std::string text = "TEXT";
static const std::string strXml1_ = R"(
<root>
    <doc name="doc">
        <a a="a">)";
static const std::string strXml2_ = R"(</a><duplicate/><duplicate/>
        <values int="314" double="3.14" string="abc"/>
        <int>314</int>
        <double>3.14</double>
        <string>abc</string>
        <bool>yes</bool>
        <empty></empty>
    </doc>
</root>)";
static const auto strXml = strXml1_ + text + strXml2_;

struct test_MinidomParser final
{
    mutable xml::lite::MinidomParser xmlParser;
    const xml::lite::Element& getRootElement() const
    {
        io::StringStream ss;
        ss.stream() << strXml;

        xmlParser.parse(ss);
        return xml::lite::getRootElement(xmlParser.getDocument());
    }
    xml::lite::Element& getRootElement()
    {
        io::StringStream ss;
        ss.stream() << strXml;

        xmlParser.parse(ss);
        return xml::lite::getRootElement(xmlParser.getDocument());
    }
};

TEST_CASE(test_CloneCopy_root_encoding)
{
    {
        test_MinidomParser xmlParser;
        auto& root_ = xmlParser.getRootElement();
        root_.setCharacterData(U8("abc"));
        const auto& root = root_;
        test_assert_utf8(testName, root);

        xml::lite::Element copy;
        copy.clone(root);
        copy.clearChildren();
        test_assert_utf8(testName, copy);
        copy.setCharacterData("xyz");
        test_assert_platform(testName, copy);
        test_assert_utf8(testName, root);

        root_.setCharacterData("123");
        test_assert_platform(testName, root);
    }
    {
        test_MinidomParser xmlParser;
        auto& root_ = xmlParser.getRootElement();
        root_.setCharacterData(U8("abc"));
        const auto& root = root_;

        xml::lite::Element copy;
        copy.clone(root);
        copy.clearChildren();
        test_assert_utf8(testName, copy);
        copy.setCharacterData("xyz", xml::lite::StringEncoding::Windows1252);
        TEST_ASSERT(copy.getEncoding() == xml::lite::StringEncoding::Windows1252);
        test_assert_utf8(testName, root);
        TEST_ASSERT(root.getEncoding() != copy.getEncoding());

        root_.setCharacterData("123");
        test_assert_platform(testName, root);
        TEST_ASSERT(copy.getEncoding() == xml::lite::StringEncoding::Windows1252);
    }
}

TEST_CASE(test_CloneCopy_copy_encoding)
{
    test_MinidomParser xmlParser;
    auto& root_ = xmlParser.getRootElement();
    root_.setCharacterData("abc");
    const auto& root = root_;

    xml::lite::Element copy;
    copy.clone(root);
    copy.clearChildren();
    copy.setCharacterData(U8("xyz"));
    test_assert_utf8(testName, copy);
    test_assert_platform(testName, root);
}

TEST_CASE(test_getRootElement)
{
    io::StringStream ss;
    ss.stream() << strXml;
    TEST_ASSERT_EQ(ss.stream().str(), strXml);

    xml::lite::MinidomParser xmlParser;
    xmlParser.parse(ss);
    const auto pRoot = &getRootElement(getDocument(xmlParser));
    TEST_ASSERT(pRoot != nullptr);
}

TEST_CASE(test_getElementsByTagName)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();
    
    {
        const auto aElements = root.getElementsByTagName("a", true /*recurse*/);
        TEST_ASSERT_EQ(aElements.size(), static_cast<size_t>(1));
        const auto& a = *(aElements[0]);

        const auto characterData = a.getCharacterData();
        TEST_ASSERT_EQ(characterData, text);
    }
    
    const auto docElements = root.getElementsByTagName("doc");
    TEST_ASSERT_FALSE(docElements.empty());
    TEST_ASSERT_EQ(docElements.size(), static_cast<size_t>(1));
    {
        const auto aElements = docElements[0]->getElementsByTagName("a");
        TEST_ASSERT_EQ(aElements.size(), static_cast<size_t>(1));
        const auto& a = *(aElements[0]);

        const auto characterData = a.getCharacterData();
        TEST_ASSERT_EQ(characterData, text);
        test_assert_platform(testName, a);
    }
}

TEST_CASE(test_getElementsByTagName_duplicate)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

    {
        const auto duplicateElements = root.getElementsByTagName("duplicate", true /*recurse*/);
        TEST_ASSERT_EQ(duplicateElements.size(), static_cast<size_t>(2));
        const auto& duplicate = *(duplicateElements[0]);

        const auto characterData = duplicate.getCharacterData();
        TEST_ASSERT_TRUE(characterData.empty());
    }

    const auto docElements = root.getElementsByTagName("doc");
    TEST_ASSERT_FALSE(docElements.empty());
    TEST_ASSERT_EQ(docElements.size(), static_cast<size_t>(1));
    {
        const auto duplicateElements = docElements[0]->getElementsByTagName("duplicate");
        TEST_ASSERT_EQ(duplicateElements.size(), static_cast<size_t>(2));
        const auto& duplicate = *(duplicateElements[0]);

        const auto characterData = duplicate.getCharacterData();
        TEST_ASSERT_TRUE(characterData.empty());
    }
}

TEST_CASE(test_getElementByTagName)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

    {
        const auto& a = root.getElementByTagName("a", true /*recurse*/);
        const auto characterData = a.getCharacterData();
        TEST_ASSERT_EQ(characterData, text);
    }

    const auto& doc = root.getElementByTagName("doc");
    {
        const auto& a = doc.getElementByTagName("a");
        const auto characterData = a.getCharacterData();
        TEST_ASSERT_EQ(characterData, text);
    }
}

TEST_CASE(test_getElementByTagName_nothrow)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

    {
        const auto pNotFound = root.getElementByTagName(std::nothrow, "not_found", true /*recurse*/);
        TEST_ASSERT_NULL(pNotFound);
        const auto pDuplicate = root.getElementByTagName(std::nothrow, "duplicate", true /*recurse*/);
        TEST_ASSERT_NULL(pDuplicate);
    }

    const auto& doc = root.getElementByTagName("doc");
    {
        const auto pNotFound = doc.getElementByTagName(std::nothrow, "not_found");
        TEST_ASSERT_NULL(pNotFound);
        const auto pDuplicate = doc.getElementByTagName(std::nothrow, "duplicate");
        TEST_ASSERT_NULL(pDuplicate);
    }
}

TEST_CASE(test_getElementByTagName_throw)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();
    
    TEST_SPECIFIC_EXCEPTION(root.getElementByTagName("not_found", true /*recurse*/), xml::lite::XMLException);
    TEST_SPECIFIC_EXCEPTION(root.getElementByTagName("duplicate", true /*recurse*/), xml::lite::XMLException);

    const auto& doc = root.getElementByTagName("doc");
    TEST_SPECIFIC_EXCEPTION(doc.getElementByTagName("not_found"), xml::lite::XMLException);
    TEST_SPECIFIC_EXCEPTION(doc.getElementByTagName("duplicate"), xml::lite::XMLException);
}

TEST_CASE(test_getValue)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

     using namespace xml::lite;
    {
        const auto& e = root.getElementByTagName("int", true /*recurse*/);
        int value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(314, value);

        value = getValue<int>(e);
        TEST_ASSERT_EQ(314, value);
    }
    {
        const auto& e = root.getElementByTagName("double", true /*recurse*/);
        double value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(3.14, value);

        value = getValue<double>(e);
        TEST_ASSERT_EQ(3.14, value);
    }
    {
        const auto& e = root.getElementByTagName("string", true /*recurse*/);
        std::string value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ("abc", value);

        value = getValue<std::string>(e);
        TEST_ASSERT_EQ("abc", value);
    }
    {
        const auto& e = root.getElementByTagName("bool", true /*recurse*/);
        auto toType = [](const std::string& value) { return value == "yes"; };

        bool value = false;
        auto result = castValue(e, value, toType);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(true, value);
        value = castValue(e, toType);
        TEST_ASSERT_EQ(true, value);

        std::string strValue;
        result = getValue(e, strValue);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ("yes", strValue);
        strValue = getValue<std::string>(e);
        TEST_ASSERT_EQ("yes", strValue);
    }
    {
        const auto& e = root.getElementByTagName("empty", true /*recurse*/);
        std::string value;
        auto result = getValue(e, value);
        TEST_ASSERT_FALSE(result);
        value = e.getCharacterData();
        TEST_ASSERT_TRUE(value.empty());
    }
}


TEST_CASE(test_getValueFailure)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

    {
        const auto& e = root.getElementByTagName("string", true /*recurse*/);
        int value;
        const auto result = getValue(e, value);
        TEST_ASSERT_FALSE(result);
    }
    {
        const auto& e = root.getElementByTagName("string", true /*recurse*/);
        double value;
        const auto result = getValue(e, value);
        TEST_ASSERT_FALSE(result);
    }
    {
        const auto& e = root.getElementByTagName("empty", true /*recurse*/);
        std::string value;
        const auto result = getValue(e, value);
        TEST_ASSERT_FALSE(result);
    }
}

TEST_CASE(test_getValueThrows)
{
    test_MinidomParser xmlParser;
    const auto& root = xmlParser.getRootElement();

     using namespace xml::lite;
    {
        const auto& e = root.getElementByTagName("string", true /*recurse*/);
        TEST_SPECIFIC_EXCEPTION(getValue<int>(e), except::BadCastException);
    }
    {
        const auto& e = root.getElementByTagName("empty", true /*recurse*/);
        TEST_SPECIFIC_EXCEPTION(getValue<std::string>(e), except::BadCastException);
    }
}

TEST_CASE(test_setValue)
{
    test_MinidomParser xmlParser;
    auto& root = xmlParser.getRootElement();

    {
        auto& e = root.getElementByTagName("int", true /*recurse*/);
        setValue(e, 123);
        int value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(123, value);
    }
    {
        auto& e = root.getElementByTagName("double", true /*recurse*/);
        setValue(e, 1.23);
        double value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(1.23, value);
    }
    {
        auto& e = root.getElementByTagName("string", true /*recurse*/);
        setValue(e, "xyz");
        std::string value;
        const auto result = getValue(e, value);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ("xyz", value);
    }
    {
        auto& e = root.getElementByTagName("bool", true /*recurse*/);
        
        auto toString = [](const bool& value) { return value ? "yes" : "no"; };
        setValue(e, true, toString);
        
        auto toType = [](const std::string& value) { return value == "yes"; };
        bool value = false;
        auto result = castValue(e, value, toType);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ(true, value);
        std::string strValue;
        result = getValue(e, strValue);
        TEST_ASSERT_TRUE(result);
        TEST_ASSERT_EQ("yes", strValue);
    }
}

int main(int, char**)
{
    TEST_CHECK(test_CloneCopy_root_encoding);
    TEST_CHECK(test_CloneCopy_copy_encoding);

    TEST_CHECK(test_getRootElement);
    TEST_CHECK(test_getElementsByTagName);
    TEST_CHECK(test_getElementsByTagName_duplicate);
    TEST_CHECK(test_getElementByTagName);
    TEST_CHECK(test_getElementByTagName_nothrow);    
    TEST_CHECK(test_getElementByTagName_throw);

    TEST_CHECK(test_getValue);
    TEST_CHECK(test_getValueFailure);
    TEST_CHECK(test_getValueThrows);
    TEST_CHECK(test_setValue);
}
