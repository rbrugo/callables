######################################################################
# @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
# @file        : CMakeLists
# @created     : Monday Aug 01, 2022 18:47:17 CEST
######################################################################
cmake_minimum_required(VERSION 3.16.2)

project(callables CXX)
include(cmake/general.cmake)

add_library(callables INTERFACE)
target_compile_features(callables INTERFACE cxx_std_23)
target_include_directories(callables INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

enable_testing()
add_subdirectory(test)
