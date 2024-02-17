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
        test_deserializing.cpp
        test_overflow.cpp
    )
    target_link_libraries(jungles_bitfield_runtime_tests PRIVATE Catch2::Catch2WithMain jungles::bitfield)
    target_compile_options(jungles_bitfield_runtime_tests PRIVATE -Wall $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>)
    add_test(NAME jungles_bitfield_runtime 
        COMMAND valgrind --leak-check=full $<TARGET_FILE:jungles_bitfield_runtime_tests>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

endmacro()

function(CompileTimePositiveTest name test_source_file)

    set(exec_name test_${name})
    add_executable(${exec_name} ${test_source_file})
    target_link_libraries(${exec_name} PRIVATE jungles::bitfield)
    target_compile_options(${exec_name} PRIVATE -Wall -Wextra)
    set_target_properties(${exec_name} PROPERTIES 
        EXCLUDE_FROM_ALL TRUE 
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${name}
        COMMAND cmake --build . --target ${exec_name}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_tests_properties(${name} PROPERTIES LABELS "Portable")

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

    add_test(NAME ${name}
        COMMAND ${CMAKE_COMMAND} --build . --target ${exec_name} --config $<CONFIGURATION>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${pass_regex})
    set_tests_properties(${name} PROPERTIES LABELS "Portable")

endfunction()

function(CreateCompileTimeTests)

    CompileTimePositiveTest(various_id_types test_various_id_types.cpp)

    CompileTimeNegativeTest(
        wrong_id_when_calling_at
        "Bitfields<unsigned char, Field<10, 3>, Field<20, 5>>{}.at<4>()"
        ".*Field ID not found.*")

    CompileTimeNegativeTest(
        wrong_id_when_calling_extract
        "Bitfields<unsigned char, Field<5, 3>, Field<3, 5>>{}.extract<4>()"
        ".*Field ID not found.*")

    CompileTimeNegativeTest(
        ids_must_not_duplicate
        "Bitfields<unsigned char, Field<10, 3>, Field<10, 5>>{}"
        ".*Field IDs must not duplicate.*")

    CompileTimeNegativeTest(
        bits_not_occupied
        "Bitfields<unsigned char, Field<0, 3>, Field<10, 4>>{}"
        ".*Accumulated bit size is not equal to underlying type's bit size.*")

    CompileTimeNegativeTest(
        bitfield_group_doesnt_fit
        "Bitfields<unsigned char, Field<0, 5>, Field<10, 4>>{}"
        ".*Accumulated bit size is not equal to underlying type's bit size.*")

endfunction()

function(CreatePortabilityTests)

    ProvideLlvm13(llvm_path)

    add_test(
        NAME build_with_clang13
        COMMAND ctest --build-and-test ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/clang13_build
                      --build-generator ${CMAKE_GENERATOR}
                      --build-options -DLLVM_TOOLCHAIN_PATH=${llvm_path}
                                      -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/llvm_toolchain_file.cmake
                                      -DCMAKE_BUILD_TYPE=Release
                                      -DJUNGLES_BITFIELD_ENABLE_TESTING=ON
                                      -DJUNGLES_BITFIELD_ENABLE_PORTABILITY_TESTS=OFF
    )

    set_tests_properties(build_with_clang13 PROPERTIES LABELS "Ubuntu20")

endfunction()

################################################################################
# Main script
################################################################################

include(dependencies.cmake)

DownloadAndPopulateCatch2()
CreateRuntimeTests()
CreateCompileTimeTests()

option(JUNGLES_BITFIELD_ENABLE_PORTABILITY_TESTS "Includes portability tests" OFF)

if(JUNGLES_BITFIELD_ENABLE_PORTABILITY_TESTS)
    CreatePortabilityTests()
endif()
