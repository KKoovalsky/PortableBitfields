function(ProvideLlvm13 llvm_source_dir_external_variable)
    
    include(FetchContent)

    FetchContent_Declare(
        Llvm
        URL https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
        URL_HASH SHA256=2c2fb857af97f41a5032e9ecadf7f78d3eff389a5cd3c9ec620d24f134ceb3c8
    )

    FetchContent_MakeAvailable(Llvm)

    FetchContent_GetProperties(Llvm SOURCE_DIR LLVM_TOOLCHAIN_SOURCE_DIR)

    set(${llvm_source_dir_external_variable} ${LLVM_TOOLCHAIN_SOURCE_DIR} PARENT_SCOPE)

endfunction()
