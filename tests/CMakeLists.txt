cmake_minimum_required(VERSION 3.21)

################################################################################
# Main script
################################################################################

macro(DownloadAndPopulateCatch2)
    
    set(CATCH_BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 self-test")

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 634cdb4efefcbd9ee8816e9507013f10de20a78d
    )
    FetchContent_MakeAvailable(Catch2)

endmacro()

macro (CreateRuntimeTests)

    add_executable(jungles_bitfield_runtime_tests
        test_operations_on_bitfields.cpp
        test_extracting.cpp
    )
    target_link_libraries(jungles_bitfield_runtime_tests PRIVATE Catch2::Catch2WithMain jungles::bitfield)
    target_compile_options(jungles_bitfield_runtime_tests PRIVATE -Wall -Wextra)
    add_test(NAME jungles_bitfield_runtime COMMAND valgrind --leak-check=full $<TARGET_FILE:jungles_bitfield_runtime_tests>)

endmacro()

################################################################################
# Main script
################################################################################

DownloadAndPopulateCatch2()
CreateRuntimeTests()
