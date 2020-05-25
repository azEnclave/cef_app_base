cmake_minimum_required(VERSION 2.8.12.1 FATAL_ERROR)

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0074 NEW)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# build directory config
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

set(CMAKE_BINARY_DIR ${BUILD_DIR})
set(CMAKE_BUILD_FILES_DIRECTORY ${BUILD_DIR})
set(CMAKE_BUILD_DIRECTORY ${BUILD_DIR})

# Project name.
project(custom_cef_app)

# Target executable names.
set(PROJECT_TARGET_EXECUTABLE_NAME ${PROJECT_NAME})

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# COMPILATION FLAG SETTINGS
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -pipe")
set(GCC_COVERAGE_LINK_FLAGS "")

# SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
set(CMAKE_CXX_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${GCC_COVERAGE_LINK_FLAGS}")

#
# CEF configuration.
#

# Specify the CEF distribution version.
# NOT USED FOR ANYTHING UNLESS YOU DO NOT HAVE CEF_ROOT set.
# This should only be used to know with what version of CEF was the application built.
set(CEF_VERSION "cef_binary_81.3.10+gb223419+chromium-81.0.4044.138")

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(CEF_PLATFORM "macosx64")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    if(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "linux64")
    else()
        set(CEF_PLATFORM "linux32")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "windows64")
    else()
        set(CEF_PLATFORM "windows32")
    endif()
endif()

# Set the Cmake folder for configs.
# Add the /cmake directory from the CEF_ROOT environment variable to the module path. This will be used for
# detecting the CEF version and to use different cmake functions within.
if(NOT DEFINED "ENV{CEF_ROOT}")
    message(STATUS "CEF_ROOT environment variable not SET: --[$ENV{CEF_ROOT}]--")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{CEF_ROOT}/cmake")

# Execute FindCEF.cmake which must exist in CMAKE_MODULE_PATH.
find_package(CEF REQUIRED)

# Include the libcef_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
# THIS IS SET FROM THE CEF_ROOT BINARY DISTRIBUTION
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# Allow includes relative to the current source directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# include scripts from the base CEF git repo
include_directories(${CEF_INCLUDE_PATH})

# Include application targets.
# Comes from the <target>/CMakeLists.txt file in the current directory.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
endif()

# Display configuration settings.
PRINT_CEF_CONFIG()
