#
# Copyright by The HDF Group.
# All rights reserved.
#
# This file is part of HDF5.  The full HDF5 copyright notice, including
# terms governing use, modification, and redistribution, is contained in
# the COPYING file, which can be found at the root of the source code
# distribution tree, or in https://www.hdfgroup.org/licenses.
# If you do not have access to either file, you may request a copy from
# help@hdfgroup.org.
#
option (USE_LIBAEC_STATIC "Use static AEC library " OFF)
option (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 0)
option (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 0)

option (BUILD_ZLIB_WITH_FETCHCONTENT "Use FetchContent to use original source files" OFF)
if (BUILD_ZLIB_WITH_FETCHCONTENT)
  set (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 1)
  if (NOT ZLIB_USE_LOCALCONTENT)
    set (ZLIB_URL ${ZLIB_TGZ_ORIGPATH}/${ZLIB_TGZ_ORIGNAME})
  else ()
    set (ZLIB_URL ${TGZPATH}/${ZLIB_TGZ_ORIGNAME})
  endif ()
  if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
    message (VERBOSE "Filter ZLIB file is ${ZLIB_URL}")
  endif ()
endif ()

option (BUILD_SZIP_WITH_FETCHCONTENT "Use FetchContent to use original source files" OFF)
if (BUILD_SZIP_WITH_FETCHCONTENT)
  set (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 1)
  if (NOT LIBAEC_USE_LOCALCONTENT)
    set (SZIP_URL ${LIBAEC_TGZ_ORIGPATH}/${LIBAEC_TGZ_ORIGNAME})
  else ()
    set (SZIP_URL ${TGZPATH}/${LIBAEC_TGZ_ORIGNAME})
  endif ()
  if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
    message (VERBOSE "Filter SZIP file is ${SZIP_URL}")
  endif ()
endif ()

include (ExternalProject)
#option (HDF5_ALLOW_EXTERNAL_SUPPORT "Allow External Library Building (NO GIT TGZ)" "NO")
set (HDF5_ALLOW_EXTERNAL_SUPPORT "NO" CACHE STRING "Allow External Library Building (NO GIT TGZ)")
set_property (CACHE HDF5_ALLOW_EXTERNAL_SUPPORT PROPERTY STRINGS NO GIT TGZ)
if (HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  set (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 1)
  set (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 1)
  if (HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT")
    set (ZLIB_URL ${ZLIB_GIT_URL} CACHE STRING "Path to zlib git repository")
    set (ZLIB_BRANCH ${ZLIB_GIT_BRANCH})
    set (SZIP_URL ${SZIP_GIT_URL} CACHE STRING "Path to szip git repository")
    set (SZIP_BRANCH ${SZIP_GIT_BRANCH})
  elseif (HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    if (NOT TGZPATH)
      set (TGZPATH ${HDF5_SOURCE_DIR})
    endif ()
    if (NOT BUILD_ZLIB_WITH_FETCHCONTENT)
      set (ZLIB_URL ${TGZPATH}/${ZLIB_TGZ_NAME})
    endif ()
    if (ZLIB_USE_LOCALCONTENT)
      if (NOT EXISTS "${ZLIB_URL}")
        set (HDF5_ENABLE_Z_LIB_SUPPORT OFF CACHE BOOL "" FORCE)
        message (VERBOSE "Filter ZLIB file ${ZLIB_URL} not found")
      endif ()
    endif ()
    if (NOT BUILD_SZIP_WITH_FETCHCONTENT)
      set (SZIP_URL ${TGZPATH}/${SZAEC_TGZ_NAME})
    endif ()
    if (LIBAEC_USE_LOCALCONTENT)
      if (NOT EXISTS "${SZIP_URL}")
        set (HDF5_ENABLE_SZIP_SUPPORT OFF CACHE BOOL "" FORCE)
        message (VERBOSE "Filter SZIP file ${SZIP_URL} not found")
      endif ()
    endif ()
  else ()
    set (ZLIB_USE_EXTERNAL 0)
    set (SZIP_USE_EXTERNAL 0)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
option (HDF5_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" ON)
if (HDF5_ENABLE_Z_LIB_SUPPORT)
  if (NOT H5_ZLIB_HEADER)
    if (NOT ZLIB_USE_EXTERNAL)
      find_package (ZLIB NAMES ${ZLIB_PACKAGE_NAME}${HDF_PACKAGE_EXT} COMPONENTS static shared)
      if (NOT ZLIB_FOUND)
        find_package (ZLIB) # Legacy find
      endif ()
      if (ZLIB_FOUND)
        set (H5_ZLIB_HEADER "zlib.h")
        set (ZLIB_INCLUDE_DIR_GEN ${ZLIB_INCLUDE_DIR})
        set (ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIR})
        set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${ZLIB_LIBRARIES})
      endif ()
    else ()
      if (BUILD_ZLIB_WITH_FETCHCONTENT)
        # Only tgz files available
        ORIGINAL_ZLIB_LIBRARY ("TGZ")
        message (VERBOSE "HDF5_ZLIB is built from fetch content")
        set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${ZLIB_STATIC_LIBRARY})
      elseif (HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        EXTERNAL_ZLIB_LIBRARY (${HDF5_ALLOW_EXTERNAL_SUPPORT})
        message (VERBOSE "Filter HDF5_ZLIB is built")
        set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${ZLIB_STATIC_LIBRARY})
      endif ()
    endif ()
  else ()
    # This project is being called from within another and ZLib is already configured
    set (H5_HAVE_FILTER_DEFLATE 1)
    set (H5_HAVE_ZLIB_H 1)
    set (H5_HAVE_LIBZ 1)
  endif ()
  if (ZLIB_FOUND)
    set (H5_HAVE_FILTER_DEFLATE 1)
    set (H5_HAVE_ZLIB_H 1)
    set (H5_HAVE_LIBZ 1)
    if (H5_HAVE_FILTER_DEFLATE)
      set (EXTERNAL_FILTERS "${EXTERNAL_FILTERS} DEFLATE")
    endif ()
    INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
    message (VERBOSE "Filter HDF5_ZLIB is ON")
  else ()
    message (WARNING " ZLib support in HDF5 was enabled but not found")
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
option (HDF5_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)
if (HDF5_ENABLE_SZIP_SUPPORT)
  option (HDF5_ENABLE_SZIP_ENCODING "Use SZip Encoding" ON)
  if (NOT SZIP_USE_EXTERNAL)
    set(SZIP_FOUND FALSE)
    set(libaec_USE_STATIC_LIBS ${USE_LIBAEC_STATIC})
    find_package (libaec 1.0.5 CONFIG)
    if (SZIP_FOUND)
      set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_LIBRARIES})
    endif ()
    if (NOT SZIP_FOUND)
      find_package (SZIP NAMES ${LIBAEC_PACKAGE_NAME}${HDF_PACKAGE_EXT} COMPONENTS static shared)
      if (NOT SZIP_FOUND)
        find_package (SZIP) # Legacy find
      endif ()
    endif ()
    if (SZIP_FOUND)
      set (SZIP_INCLUDE_DIR_GEN ${SZIP_INCLUDE_DIR})
      set (SZIP_INCLUDE_DIRS ${SZIP_INCLUDE_DIRS} ${SZIP_INCLUDE_DIR})
      set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_LIBRARIES})
    endif ()
  else ()
    if (BUILD_SZIP_WITH_FETCHCONTENT)
      # Only tgz files available
      ORIGINAL_SZIP_LIBRARY ("TGZ" ${HDF5_ENABLE_SZIP_ENCODING})
      message (VERBOSE "SZIP is built from fetch content")
      message (VERBOSE "... with library AEC")
      set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_STATIC_LIBRARY})
    elseif (HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF5_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      EXTERNAL_SZIP_LIBRARY (${HDF5_ALLOW_EXTERNAL_SUPPORT} ${HDF5_ENABLE_SZIP_ENCODING})
      message (VERBOSE "Filter SZIP is built")
      message (VERBOSE "... with library AEC")
      set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_STATIC_LIBRARY})
    endif ()
  endif ()
  if (SZIP_FOUND)
    set (H5_HAVE_FILTER_SZIP 1)
    set (H5_HAVE_SZLIB_H 1)
    set (H5_HAVE_LIBSZ 1)
    INCLUDE_DIRECTORIES (${SZIP_INCLUDE_DIRS})
    message (VERBOSE "Filter SZIP is ON")
    if (H5_HAVE_FILTER_SZIP)
      set (EXTERNAL_FILTERS "${EXTERNAL_FILTERS} DECODE")
    endif ()
    if (HDF5_ENABLE_SZIP_ENCODING)
      set (H5_HAVE_SZIP_ENCODER 1)
      set (EXTERNAL_FILTERS "${EXTERNAL_FILTERS} ENCODE")
    endif ()
  else ()
    message (WARNING "SZIP support in HDF5 was enabled but not found")
  endif ()
endif ()
