cmake_minimum_required(VERSION 3.25)

project(searchengine VERSION 0.1)

configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_CXX_STANDARD 17)

set (MY_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

add_subdirectory (src)

add_subdirectory(Google_tests)



