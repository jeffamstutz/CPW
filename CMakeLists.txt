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

cpw_setup_build_type()

# we are only building tests, should probably stick with 'Release'
mark_as_advanced(CMAKE_BUILD_TYPE)

install(DIRECTORY CPW DESTINATION include)

##############################################################
# Build tests
##############################################################

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests)

include_directories(${CMAKE_SOURCE_DIR})

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

# parallel_for
cpw_add_test(parallel_for
  tests/doctest.h
  tests/parallel_for.cpp
  CPW/parallel_for.h
)

# async
cpw_add_test(async
  tests/doctest.h
  tests/async.cpp
  CPW/async.h
)
