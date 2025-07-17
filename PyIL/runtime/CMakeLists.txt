cmake_minimum_required(VERSION 3.10.0)

project(PyIL-RT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps)
add_library(
    ${PROJECT_NAME} SHARED

    il2cpp/utils/debug.cc    

    il2cpp/assemblies.cc
    il2cpp/state.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    minhook
)