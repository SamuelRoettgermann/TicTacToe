cmake_minimum_required(VERSION 3.28)
project(TicTacToe)

# try to prevent in-source builds
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate 'build' directory and build in there.")
endif()

# set default c++ standard for compile targets
set(CMAKE_CXX_STANDARD 20)

# require C++20, don't allow falling back to older version of standard
set(CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

#------- tweaks for an improved build experience -------

# so language servers like clangd know what the buildsystem does
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# display many warnings during compilation
add_compile_options(-Wall -Wextra -Wconversion -pedantic -Wfatal-errors)

add_compile_options(-march=native)

#------- code inclusion -------

add_subdirectory("src")