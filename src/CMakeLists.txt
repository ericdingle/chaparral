cmake_minimum_required(VERSION 3.13.4)

project(chaparral)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include relative to root source dir.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Bonavista.
configure_file(Bonavista.cmake third_party/bonavista/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/bonavista" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/bonavista" )
set(bonavista_tests OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/bonavista/src)

option(chaparral_tests "Whether to create test targets." ON)
if (chaparral_tests)
  enable_testing()

  # GoogleTest.
  configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest" )
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest" )
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  add_subdirectory(third_party/googletest)
endif()

add_subdirectory(calc EXCLUDE_FROM_ALL)
add_subdirectory(executer)
