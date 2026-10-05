######################################################################
# @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
# @file        : CMakeLists
# @created     : Monday Aug 01, 2022 18:47:17 CEST
######################################################################
cmake_minimum_required(VERSION 3.30)

project(callables VERSION 0.2.1 LANGUAGES CXX)

include(GNUInstallDirs)

option(CALLABLES_BUILD_TESTS "Build the callables test suite" ${PROJECT_IS_TOP_LEVEL})

if (PROJECT_IS_TOP_LEVEL OR CALLABLES_BUILD_TESTS)
    include(cmake/general.cmake)
endif()

add_library(callables INTERFACE)
add_library(callables::callables ALIAS callables)

target_compile_features(callables INTERFACE cxx_std_26)
target_include_directories(callables
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (CALLABLES_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                              packaging                               #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS CMAKE_INSTALL_LIBDIR)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT  # no need for compatibility check regarding the machine
)

install(TARGETS callables EXPORT ${PROJECT_NAME}-targets)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NAMESPACE callables::
    FILE "${PROJECT_NAME}-targets.cmake"
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
