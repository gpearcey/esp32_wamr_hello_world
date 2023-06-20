# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

#
# Find the Espressif IDF (IoT Development Framework) path, which
# contains CMake descriptions of the Espressif toolchain.
#
find_path(IDF_PATH tools
          DOC "Espressif IoT Development Framework directory"
          PATHS $ENV{IDF_PATH})

if(NOT IDF_PATH)
    message(FATAL_ERROR "Cannot find IDF tools; try setting IDF_PATH env var")
endif()


#
# Specify IDF ISA and target explicitly.
#
set(IDF_ISA "xtensa" CACHE STRING "Instruction Set Architecture (default: xtensa)")
set(IDF_TARGET "esp32" CACHE STRING "Target (default: esp32)")
#
# Find Espressif compiler path.
#
set(ESP_COMPILER_NAME "${IDF_ISA}-${IDF_TARGET}-elf-gcc")
find_path(ESP_COMPILER_PATH "${ESP_COMPILER_NAME}"
          DOC "Espressif toolchain directory"
          PATHS $ENV{PATH}
          PATH_SUFFIXES "*")

if(NOT ESP_COMPILER_PATH)
    # It would be nice if we could find the compiler and append its path to,
    # e.g., CMAKE_PROGRAM_PATH, but that isn't compatible with some of the
    # "cmake calling cmake" stuff that the Espressif toolchain does to build
    # bootloader components.
    message(FATAL_ERROR "Cannot find ${ESP_COMPILER_NAME};"
            "try setting PATH to include, e.g.,"
            " ~/.espressif/tools/.../${IDF_ISA}-${IDF_TARGET}-elf")
endif()

#ESP project details

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set (COMPONENTS ${IDF_TARGET} main freertos esptool_py wamr)
list(APPEND EXTRA_COMPONENT_DIRS "$ENV{WAMR_PATH}/build-scripts/esp-idf")

project(wamr-blink)

