cmake_minimum_required(VERSION 3.10.0)

project(PyIL-RT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    ${PROJECT_NAME} SHARED
        
    utils/path.cc
    entry.cc
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    minhook
    ILight
    PyLight
)