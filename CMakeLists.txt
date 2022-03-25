cmake_minimum_required(VERSION 3.21)

################################################################################
# Macros
################################################################################

macro(CreateMainTarget)

    add_library(jungles_bitfield INTERFACE)
    target_include_directories(jungles_bitfield INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/)
    add_library(jungles::bitfield ALIAS jungles_bitfield)

endmacro()

################################################################################
# Main script
################################################################################

project(JunglesBitfield LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

CreateMainTarget()

set(JUNGLES_BITFIELD_ENABLE_TESTING OFF CACHE BOOL "Enables self-testing of the library")

if(JUNGLES_BITFIELD_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

