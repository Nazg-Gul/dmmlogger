# DMMLogger - A logging software for digital multimeters
# Copyright 2013 Sergey Sharybin. All rights reserved.
#
# Author: sergey.vfx@gmail.com (Sergey Sharybin)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(DMMLogger C CXX)

if(UNIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

if(MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_BINARY_DIR}/bin)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)
endif()

include_directories(.)

add_subdirectory(dmm)
add_subdirectory(tools)
add_subdirectory(ui)
add_subdirectory(third_party)
