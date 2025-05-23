cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
include(AppendCompilerFlags)

project(name_project)
set(PROJECT_VENDOR "name_author")
set(CMAKE_PROJECT_NAME "rlz")
set(PROJECT_CONTACT "project_contact")


# Change to "Debug" when you need it
set(CMAKE_BUILD_TYPE "Debug")
# Guarantee that the debug flags allow for symbols and no optimization
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (NOT CMAKE_CXX_FLAGS_DEBUG MATCHES "-g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    endif ()
    if (NOT CMAKE_CXX_FLAGS_DEBUG MATCHES "-O0")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    endif ()
endif ()

# C++11 compiler Check
if (NOT CMAKE_CXX_COMPILER_VERSION) # work around for cmake versions smaller than 2.8.10
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CMAKE_CXX_COMPILER_VERSION)
endif ()
if (CMAKE_CXX_COMPILER MATCHES ".*clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()
if ((CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.7) OR
(CMAKE_COMPILER_IS_CLANGXX AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.2))
    message(FATAL_ERROR "Your C++ compiler does not support C++11. Please install g++ 4.7 (or greater) or clang 3.2 (or greater)")
else ()
    message(STATUS "Compiler is recent enough to support C++11.")
endif ()
if (CMAKE_COMPILER_IS_GNUCXX)
    append_cxx_compiler_flags("-std=c++11 -Wall -Wextra " "GCC" CMAKE_CXX_FLAGS)
    append_cxx_compiler_flags("-msse4.2 -O3 -ffast-math -funroll-loops -fno-omit-frame-pointer -g" "GCC" CMAKE_CXX_FLAGS_RELEASE)
else ()
    append_cxx_compiler_flags("-std=c++11" "CLANG" CMAKE_CXX_FLAGS)
    append_cxx_compiler_flags("-stdlib=libc++" "CLANG" CMAKE_CXX_FLAGS)
    append_cxx_compiler_flags("-msse4.2 -O3  -ffast-math -funroll-loops -DNDEBUG" "CLANG" CMAKE_CXX_FLAGS_RELEASE)
endif ()
include(CheckSSE)
FindSSE()
if (SSE4_2_FOUND)
    if (CMAKE_COMPILER_IS_GNUCXX)
        append_cxx_compiler_flags("-msse4.2" "GCC" CMAKE_CXX_FLAGS)
    else ()
        append_cxx_compiler_flags("-msse4.2" "CLANG" CMAKE_CXX_FLAGS)
    endif ()
    message(STATUS "CPU does support SSE4.2.")
else ()
    message(STATUS "CPU does NOT support SSE4.2")
endif ()

add_subdirectory(external/sdsl-lite-custom)

include_directories("${PROJECT_SOURCE_DIR}/external/googletest/include"
        ${CMAKE_HOME_DIRECTORY}/include
        ${CMAKE_BINARY_DIR}/external/sdsl-lite/include
        ${CMAKE_BINARY_DIR}/external/sdsl-lite/external/libdivsufsort/include/
        ${CMAKE_HOME_DIRECTORY}/external/googletest/include
        ${CMAKE_HOME_DIRECTORY}/external/sdsl-lite/external/FastPFor/headers/
        ${STXXL_INCLUDE_DIRS}
)


add_executable(rlz src/rlz.cpp)
target_link_libraries(rlz sdsl divsufsort divsufsort64)

add_executable(random_integers src/random_integers.cpp)
target_link_libraries(random_integers sdsl divsufsort divsufsort64)

