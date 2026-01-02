######################################################################
# @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
# @file        : CMakeLists
# @created     : Monday Aug 01, 2022 18:47:17 CEST
######################################################################
cmake_minimum_required(VERSION 4.2.0)

project(callables VERSION 0.2.1 LANGUAGES CXX)
include(cmake/general.cmake)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

add_library(callables INTERFACE)
target_compile_features(callables INTERFACE cxx_std_23)
target_include_directories(callables
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
)

enable_testing()
add_subdirectory(test)

# Modules
# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)
add_library(callables_modules)
target_compile_features(callables_modules PUBLIC cxx_std_23)
target_sources(callables_modules
    PUBLIC
    FILE_SET CXX_MODULES FILES
        "modules/arithmetic.cppm"
        "modules/bit_operators.cppm"
        "modules/combinators.cppm"
        "modules/comparison.cppm"
        "modules/format.cppm"
        "modules/identity.cppm"
        "modules/math.cppm"
        "modules/nullable.cppm"
        "modules/functions.cppm"
        "modules/ordering.cppm"
        "modules/operators.cppm"
        "modules/functor.cppm"
        "modules/callables.cppm"
)
target_link_libraries(callables_modules PUBLIC callables)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(callables_modules PRIVATE -fdiagnostics-color=always )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(callables_modules PRIVATE -fcolor-diagnostics)
endif()


# include(GenerateExportHeader)
# generate_export_header(callables_modules)
# target_sources(callables_modules
#     PUBLIC
#         FILE_SET HEADERS
#         BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
#         FILES
#             ${CMAKE_CURRENT_BINARY_DIR}/callables_modules_export.h
# )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                              packaging                               #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(TARGETS callables callables_modules
    EXPORT ${PROJECT_NAME}-targets
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/callables/src
    FILE_SET HEADERS     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NAMESPACE callables::
    FILE "${PROJECT_NAME}-targets.cmake"
    CXX_MODULES_DIRECTORY .
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
