cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clang-tidy.
set(BUILD_SHARED_LIBS OFF) # We expect external libraries to be linked
                           # statically.
set(CMAKE_CXX_STANDARD 17) # Compile as C++17.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.
# ##############################################################################

set(PROJECT_NAME "lox-cpp")
project(${PROJECT_NAME})
message(STATUS "PROJECT_NAME: " ${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to `Debug` as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# People keep running CMake in the wrong folder, completely nuking their project
# or creating weird bugs. This checks if you're running CMake from a folder that
# already has CMakeLists.txt. Importantly, this catches the common case of
# running it from the root directory.
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt"
     PATH_TO_CMAKELISTS_TXT)

if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
  message(
    FATAL_ERROR
      "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake ..\" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those."
  )
endif()

# Expected directory structure.
set(LOX_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(LOX_CLANG_SEARCH_PATH
    "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin"
    "/usr/local/opt/llvm@14/bin" "/opt/homebrew/opt/llvm@14/bin/")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION MATCHES "^14.")
    message(
      STATUS
        "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  else()
    message(
      WARNING
        "!! We recommend that you use clang-14 for developing LOX. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, a different version."
    )
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message(
    STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
  )
else()
  message(
    WARNING
      "!! We recommend that you use clang-14 for developing LOX. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, which is not clang."
  )
endif()

# ##############################################################################
# DEPENDENCIES
# ##############################################################################

# CTest
enable_testing()

# ##############################################################################

# ##############################################################################
# COMPILER SETUP
# ##############################################################################
if(NOT DEFINED LOX_SANITIZER)
  set(LOX_SANITIZER address)
endif()

message("Build mode: ${CMAKE_BUILD_TYPE}")
message("${LOX_SANITIZER} sanitizer will be enabled in debug mode.")

# Compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes") # TODO:
                                                                      # remove
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=${lox_SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls"
)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# Output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(LOX_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(LOX_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
set(LOX_THIRD_PARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party)

include_directories(${LOX_SRC_INCLUDE_DIR} ${LOX_TEST_INCLUDE_DIR}
                    ${LOX_THIRD_PARTY_INCLUDE_DIR})
include_directories(BEFORE src) # This is needed for gtest.

function(disable_target_warnings NAME)
  target_compile_options(${NAME} PRIVATE "-w")
endfunction()

add_library(
  lox STATIC
  ${PROJECT_SOURCE_DIR}/src/Lox.cc
  ${PROJECT_SOURCE_DIR}/src/Token.cc
  ${PROJECT_SOURCE_DIR}/src/TokenType.cc
  ${PROJECT_SOURCE_DIR}/src/Scanner.cc
  ${PROJECT_SOURCE_DIR}/src/Object.cc
  ${PROJECT_SOURCE_DIR}/src/AstPrinter.cc
  ${PROJECT_SOURCE_DIR}/src/Expression.cc)

# Other CMake modules MUST BE ADDED AFTER CONFIGURING COMPILER PARAMETERS
# ##############################################################################
set(CMAKE_MODULE_PATH "${LOX_BUILD_SUPPORT_DIR}/cmake;${CMAKE_MODULE_PATH}")
find_package(LibElf)
find_package(LibDwarf)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
