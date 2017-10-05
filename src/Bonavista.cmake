cmake_minimum_required(VERSION 2.8.12)

project(bonavista NONE)

include(ExternalProject)
ExternalProject_Add(bonavista
  GIT_REPOSITORY    https://github.com/ericdingle/bonavista.git
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bonavista"
  BINARY_DIR        ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
