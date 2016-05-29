## ========================================================================== ##
## The MIT License (MIT)                                                      ##
##                                                                            ##
## Copyright (c) 2016 Jefferson Amstutz                                       ##
##                                                                            ##
## Permission is hereby granted, free of charge, to any person obtaining a    ##
## copy of this software and associated documentation files (the "Software"), ##
## to deal in the Software without restriction, including without limitation  ##
## the rights to use, copy, modify, merge, publish, distribute, sublicense,   ##
## and/or sell copies of the Software, and to permit persons to whom the      ##
## Software is furnished to do so, subject to the following conditions:       ##
##                                                                            ##
## The above copyright notice and this permission notice shall be included in ##
## in all copies or substantial portions of the Software.                     ##
##                                                                            ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    ##
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    ##
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        ##
## DEALINGS IN THE SOFTWARE.                                                  ##
## ========================================================================== ##

##############################################################
# CMake configuration
##############################################################

cmake_minimum_required(VERSION 2.8)

project(CPW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(cmake/CPW.cmake)

# force 'Release' build type on initial configuration (otherwise is empty)
set(CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo")
if (WIN32)
  if (NOT DEFAULT_CMAKE_CONFIGURATION_TYPES_SET)
    set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}"
        CACHE STRING "List of generated configurations." FORCE)
    set(DEFAULT_CMAKE_CONFIGURATION_TYPES_SET ON CACHE INTERNAL
        "Default CMake configuration types set.")
  endif()
else()
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
        "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY 
                 STRINGS ${CONFIGURATION_TYPES})
  endif()
endif()

# we are only building tests, should probably stick with 'Release'
mark_as_advanced(CMAKE_BUILD_TYPE)

##############################################################
# Build tests
##############################################################

set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(
  ${CMAKE_SOURCE_DIR}
)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  include(cmake/icc.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  include(cmake/gcc.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  include(cmake/clang.cmake)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  include(cmake/msvc.cmake)
endif()

enable_testing()

cpw_configure_tasking_system()

add_executable(test_parallel_for tests/parallel_for.cpp CPW/parallel_for.h)

if(DEFINED TASKING_SYSTEM_LIBS)
  target_link_libraries(test_parallel_for ${TASKING_SYSTEM_LIBS})
endif()

add_test(parallel_for test_parallel_for)
