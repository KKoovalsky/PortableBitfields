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
        test_serializing.cpp
        test_zero_initialized.cpp
        test_overflow.cpp
    )
    target_link_libraries(jungles_bitfield_runtime_tests PRIVATE Catch2::Catch2WithMain jungles::bitfield)
    target_compile_options(jungles_bitfield_runtime_tests PRIVATE -Wall -Wextra)
    add_test(NAME jungles_bitfield_runtime COMMAND valgrind --leak-check=full $<TARGET_FILE:jungles_bitfield_runtime_tests>)

endmacro()

function(CompileTimePositiveTest name test_source_file)

    set(exec_name test_${name})
    add_executable(${exec_name} ${test_source_file})
    target_link_libraries(${exec_name} PRIVATE jungles::bitfield)
    target_compile_options(${exec_name} PRIVATE -Wall -Wextra)
    set_target_properties(${exec_name} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${name} COMMAND cmake --build . --target ${exec_name})

endfunction()

function(CompileTimeNegativeTest name test_body pass_regex)

    # Used within the *.in file
    set(JUNGLES_BITFIELD_STATIC_ASSERT_TEST_BODY ${test_body})
    set(output_file_name test_${name}.cpp)
    configure_file(static_assertion_test.cpp.in ${output_file_name})

    set(exec_name test_${name})
    add_executable(${exec_name} ${output_file_name})

    # target_include_directories(${exec_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(${exec_name} PRIVATE jungles::bitfield)
    set_target_properties(${exec_name} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${name} COMMAND 
        ${CMAKE_COMMAND} --build . --target ${exec_name} --config $<CONFIGURATION>)
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${pass_regex})

endfunction()

function(CreateCompileTimeTests)

    CompileTimePositiveTest(various_id_types test_various_id_types.cpp)

    CompileTimeNegativeTest(
        wrong_id_when_calling_at
        "Bitfield<unsigned char, ByteOrder::big, Field{10, 3}, Field{20, 5}>{}.at<4>()"
        ".*Field ID not found.*")

    CompileTimeNegativeTest(
        wrong_id_when_calling_extract
        "Bitfield<unsigned char, ByteOrder::big, Field{5, 3}, Field{3, 5}>{}.extract<4>()"
        ".*Field ID not found.*")

    CompileTimeNegativeTest(
        ids_must_not_duplicate
        "Bitfield<unsigned char, ByteOrder::big, Field{10, 3}, Field{10, 5}>{}"
        ".*Field IDs must not duplicate.*")

endfunction()

################################################################################
# Main script
################################################################################

DownloadAndPopulateCatch2()
CreateRuntimeTests()
CreateCompileTimeTests()
