/* =========================================================================
 * This file is part of hdf5.lite-c++
 * =========================================================================
 *
 * (C) Copyright 2022, Maxar Technologies, Inc.
 *
 * hdf5.lite-c++ is free software; you can redistribute it and/or modify
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

#include "import/std.h"
#include <std/filesystem>

#include <TestCase.h>

#include "sys/FileFinder.h"
#include "types/RowCol.h"

#include "highfive/H5Easy.hpp"
#include "highfive/H5DataSet.hpp"
#include "highfive/H5File.hpp"

#include "hdf5/lite/SpanRC.h"
#include "hdf5/lite/highfive.h"

static std::filesystem::path find_unittest_file(const std::filesystem::path& name)
{
    static const auto unittests = std::filesystem::path("modules") / "c++" / "hdf5.lite" / "unittests";
    return sys::test::findGITModuleFile("coda-oss", unittests, name);
}

TEST_CASE(test_highfive_load)
{
    static const auto path = find_unittest_file("example.h5");

    const H5Easy::File file(path.string());
    const auto lat = H5Easy::load<std::vector<double>>(file, "/g4/lat");
    TEST_ASSERT_EQ(lat.size(), 19);
    TEST_ASSERT_ALMOST_EQ(lat[0], -90.0);
    TEST_ASSERT_ALMOST_EQ(lat[0], -lat[18]);
}

TEST_CASE(test_highfive_FileException)
{
    static const std::filesystem::path path = "does not exist . h5";
    HighFive::SilenceHDF5 silencer; // no need for diagnostics, we're expecting a failure
    TEST_SPECIFIC_EXCEPTION(H5Easy::File(path.string()), HighFive::FileException);
}

TEST_CASE(test_highfive_nested)
{
    /*
    Group '/' 
    Group '/1' 
        Group '/1/bar' 
            Group '/1/bar/cat' 
                Dataset 'i' 
                    Size:  10x1
                    MaxSize:  10x1
                    Datatype:   H5T_IEEE_F64LE (double)
                    ChunkSize:  []
                    Filters:  none
                    FillValue:  0.000000
    */
    static const auto path = find_unittest_file("123_barfoo_catdog_cx.h5");

    const H5Easy::File file(path.string());
    const auto i = H5Easy::load<std::vector<std::vector<double>>>(file, "/1/bar/cat/i");
    TEST_ASSERT_EQ(i.size(), 1);
    TEST_ASSERT_EQ(i[0].size(), 10);

    const auto r = H5Easy::load<std::vector<std::vector<double>>>(file, "/1/bar/dog/r");
    TEST_ASSERT_EQ(r.size(), 1);
    TEST_ASSERT_EQ(r[0].size(), 10);
    
    TEST_ASSERT_EQ(i.size(), r.size());
    TEST_ASSERT_EQ(i[0].size(), r[0].size());
}

TEST_CASE(test_highfive_nested_small)
{
    // top group: Data
    // outer groups: 1, 2, 3, 4, 5
    // sub groups: bar, foo
    // sub-sub groups: cat, dog
    // sub-sub-sub groups: a, b, c, d
    // data: i (float array), r (float array)
    static const auto path = find_unittest_file("nested_complex_float32_data_small.h5");

    const H5Easy::File file(path.string());
    const auto i = H5Easy::load<std::vector<float>>(file, "/Data/1/bar/cat/a/i");
    TEST_ASSERT_EQ(i.size(), 10);
    auto actual = std::accumulate(i.cbegin(), i.cend(), 0.0);
    TEST_ASSERT_EQ(actual, 0.0);

    const auto r = H5Easy::load<std::vector<float>>(file, "/Data/5/foo/dog/d/r");
    TEST_ASSERT_EQ(r.size(), 10);
    actual = std::accumulate(r.cbegin(), r.cend(), 0.0);
    TEST_ASSERT_EQ(actual, 10.0);
}

TEST_CASE(test_highfive_nested_small_wrongType)
{
    static const auto path = find_unittest_file("nested_complex_float32_data_small.h5");

    const H5Easy::File file(path.string());
    HighFive::SilenceHDF5 silencer; // no need for diagnostics, we're expecting a failure
    TEST_SPECIFIC_EXCEPTION(
        H5Easy::load<std::vector<std::complex<float>>>(file, "/Data/1/bar/cat/a/i"),
        HighFive::DataSetException);
}


//*******************************************************************************

TEST_CASE(test_highfive_info)
{
    static const auto path = find_unittest_file("example.h5");
    const H5Easy::File file(path.string());

    // https://www.mathworks.com/help/matlab/ref/h5info.html
    /*
    info = struct with fields:
          Filename: '/mathworks/devel/bat/Bdoc22b/build/matlab/toolbox/matlab/demos/example.h5'
              Name: '/'
            Groups: [4x1 struct]
          Datasets: []
         Datatypes: []
             Links: []
        Attributes: [2x1 struct]
    */
    
    TEST_ASSERT_EQ(path.string(), file.getName());  
    TEST_ASSERT_EQ("/",  file.getPath());
    TEST_ASSERT_EQ(file.getNumberObjects(), 4); // 4 groups
    const auto objectNames = file.listObjectNames();
    TEST_ASSERT_EQ(objectNames.size(), 4);  // 4 groups
    for (auto&& name : objectNames)
    {
        const auto type = file.getObjectType(name);
        TEST_ASSERT(type == HighFive::ObjectType::Group);
        
        const auto group = file.getGroup(name);
        TEST_ASSERT_EQ(group.getPath(), "/" + name);   
    }
    TEST_ASSERT_EQ(file.getNumberAttributes(), 2);
}

TEST_CASE(test_highfive_groupinfo)
{
    static const auto path = find_unittest_file("example.h5");
    const H5Easy::File file(path.string());

    // https://www.mathworks.com/help/matlab/ref/h5info.html
    const auto g4 = file.getGroup("g4");
    /*
    info = struct with fields:
      Filename: '/mathworks/devel/bat/Bdoc22b/build/matlab/toolbox/matlab/demos/example.h5'
          Name: '/g4'
        Groups: []
      Datasets: [4x1 struct]
     Datatypes: []
         Links: []
    Attributes: []
    */    
    TEST_ASSERT_EQ(path.string(), g4.getFile().getName());
    const std::string groupPath("/g4");
    TEST_ASSERT_EQ(groupPath, g4.getPath());
    TEST_ASSERT_EQ(g4.getNumberObjects(), 4);  // 4 dataSets
    const auto objectNames = g4.listObjectNames();
    TEST_ASSERT_EQ(objectNames.size(), 4);  // 4 dataSets
    for (auto&& name : objectNames)
    {
        const auto type = g4.getObjectType(name);
        TEST_ASSERT(type == HighFive::ObjectType::Dataset);

        const auto dataset = g4.getDataSet(name);
        TEST_ASSERT_EQ(dataset.getPath(), groupPath + "/" +  name);
    }
}

TEST_CASE(test_highfive_datasetinfo)
{
    static const auto path = find_unittest_file("example.h5");
    const H5Easy::File file(path.string());

    // https://www.mathworks.com/help/matlab/ref/h5info.html
    const auto time = file.getDataSet("/g4/time");
    /*
    info = struct with fields:
      Filename: '/mathworks/devel/bat/Bdoc22b/build/matlab/toolbox/matlab/demos/example.h5'
          Name: 'time'
      Datatype: [1x1 struct]
     Dataspace: [1x1 struct]
     ChunkSize: 10
     FillValue: 0
       Filters: []
    Attributes: [2x1 struct]    
    */
    TEST_ASSERT_EQ(path.string(), time.getFile().getName());
    const std::string dsPath("/g4/time");
    TEST_ASSERT_EQ(dsPath, time.getPath());

    const auto dataType = time.getDataType();
    TEST_ASSERT(dataType.getClass() == HighFive::DataTypeClass::Float);

    auto dims = time.getDimensions();
    TEST_ASSERT_EQ(dims.size(), 1);
    TEST_ASSERT_EQ(10, dims[0]); // ChunkSize ???

    const auto dataSpace = time.getSpace();
    dims = dataSpace.getDimensions();
    TEST_ASSERT_EQ(dims.size(), 1);
    TEST_ASSERT_EQ(10, dims[0]);  // ChunkSize ???

    TEST_ASSERT_EQ(time.listAttributeNames().size(), 2);
}

static void read_complex(const std::string& testName, const HighFive::Group& group)
{
    const auto i = group.getDataSet("i");
    TEST_ASSERT(i.getDataType().getClass() == HighFive::DataTypeClass::Float);
    TEST_ASSERT_EQ(i.getElementCount(), 10);
    
    const auto r = group.getDataSet("r");
    TEST_ASSERT(r.getDataType().getClass() == HighFive::DataTypeClass::Float);
    TEST_ASSERT_EQ(r.getElementCount(), 10);
}
TEST_CASE(test_highfive_info_nested)
{
    /*
    Group '/' 
    Group '/1' 
        Group '/1/bar' 
            Group '/1/bar/cat' 
                Dataset 'i' 
                    Size:  10x1
                    MaxSize:  10x1
                    Datatype:   H5T_IEEE_F64LE (double)
                    ChunkSize:  []
                    Filters:  none
                    FillValue:  0.000000
    */

    static const auto path = find_unittest_file("123_barfoo_catdog_cx.h5");
    const H5Easy::File file(path.string());

    TEST_ASSERT_EQ(path.string(), file.getName());
    TEST_ASSERT_EQ("/", file.getPath());

    TEST_ASSERT_EQ(file.getNumberObjects(), 3);  // 3 groups
    auto objectNames = file.listObjectNames();
    const std::vector<std::string> expectedOuterGroupNames{"1", "2", "3"};
    TEST_ASSERT_EQ(objectNames.size(), expectedOuterGroupNames.size());
    for (auto&& outer : expectedOuterGroupNames)
    {
        const auto groupPath = "/" + outer;
        const auto group = file.getGroup(groupPath);
        TEST_ASSERT_EQ(group.getPath(), groupPath);

        TEST_ASSERT_EQ(group.getNumberObjects(), 2);  // 2 groups
        objectNames = group.listObjectNames();
        const std::vector<std::string> expectedSubGroupNames{"bar", "foo"};
        TEST_ASSERT_EQ(objectNames.size(), expectedSubGroupNames.size());
        for (auto&& subGroupName : expectedSubGroupNames)
        {
            const auto subGroup = group.getGroup(subGroupName);
            const auto subGroupPath = groupPath + "/" + subGroupName;
            TEST_ASSERT_EQ(subGroup.getPath(), subGroupPath);


            TEST_ASSERT_EQ(subGroup.getNumberObjects(), 2);  // 2 groups
            objectNames = subGroup.listObjectNames();
            const std::vector<std::string> expectedSubSubGroupNames{"cat", "dog"};
            TEST_ASSERT_EQ(objectNames.size(), expectedSubSubGroupNames.size());
            for (auto&& subSubGroupName : expectedSubSubGroupNames)
            {
                const auto subSubGroup = subGroup.getGroup(subSubGroupName);
                const auto subSubGroupPath = subGroupPath + "/" + subSubGroupName;
                TEST_ASSERT_EQ(subSubGroup.getPath(), subSubGroupPath);

                read_complex(testName, subSubGroup);
            }
        }
    }
}

//*******************************************************************************

TEST_CASE(test_highfive_create)
{
    static const auto path_ = find_unittest_file("example.h5");
    static const auto path = path_.parent_path() / "TEST_highfive_create_TMP.h5";
    H5Easy::File file(path.string(), H5Easy::File::Overwrite);
    
    const types::RowCol<size_t> dims{10, 20};
    std::vector<std::vector<float>> DS1(dims.row);
    float d = 0.0f;
    for (auto&& r : DS1)
    {
        r.resize(dims.col);
        for (size_t c = 0; c < r.size(); c++)
        {
            r[c] = d++;
        }
    }

    H5Easy::dump(file, "/DS1", DS1);
    TEST_SUCCESS;
}

TEST_CASE(test_highfive_write)
{
    static const auto path_ = find_unittest_file("example.h5");
    static const auto path = path_.parent_path() / "TEST_highfive_write_TMP.h5";

    const types::RowCol<size_t> dims{10, 20};
    std::vector<double> data_(dims.area());
    const hdf5::lite::SpanRC<double> data(data_.data(), dims);
    double d = 0.0;
    for (size_t r = 0; r<dims.row; r++)
    {
        for (size_t c = 0; c < dims.col; c++)
        {
            data(r, c) = d++;
        }    
    }    
    {
        H5Easy::File file(path.string(), H5Easy::File::Overwrite);
        const auto ds = hdf5::lite::writeDataSet(file, data, "DS1");
        const auto dimensions = ds.getDimensions();
        TEST_ASSERT_EQ(dimensions.size(), 2);
        TEST_ASSERT_EQ(dims.row, dimensions[0]);
        TEST_ASSERT_EQ(dims.col, dimensions[1]);
        TEST_ASSERT_EQ(ds.getElementCount(), data.area());
    }
    // Be sure we can read the file just written
    {
        const H5Easy::File file(path.string());

        const auto DS1 = H5Easy::load<std::vector<std::vector<double>>>(file, "/DS1");
        TEST_ASSERT_EQ(DS1.size(), dims.row);
        TEST_ASSERT_EQ(DS1[0].size(), dims.col);

        for (size_t r = 0; r < DS1.size(); r++)
        {
            for (size_t c = 0; c < DS1[r].size(); c++)
            {
                const auto expected = data(r, c);
                const auto actual = DS1[r][c];
                TEST_ASSERT_EQ(actual, expected);
            }
        }
    }
    {
        H5Easy::File file(path.string());

        std::vector<double> result;
        const auto rc = hdf5::lite::load(file, "/DS1", result);
        TEST_ASSERT(rc.dims() == dims);
        TEST_ASSERT_EQ(dims.area(), result.size());
        for (size_t i = 0; i < result.size(); i++)
        {
            const auto expected = static_cast<double>(i);
            TEST_ASSERT_EQ(result[i], expected);
        }
    }
}

TEST_MAIN(
    TEST_CHECK(test_highfive_load);
    TEST_CHECK(test_highfive_FileException);
    TEST_CHECK(test_highfive_nested);
    TEST_CHECK(test_highfive_nested_small);
    TEST_CHECK(test_highfive_nested_small_wrongType);

    TEST_CHECK(test_highfive_info);
    TEST_CHECK(test_highfive_groupinfo);
    TEST_CHECK(test_highfive_datasetinfo);
    TEST_CHECK(test_highfive_info_nested);

    //TEST_CHECK(test_highfive_create);
    //TEST_CHECK(test_highfive_write);
)