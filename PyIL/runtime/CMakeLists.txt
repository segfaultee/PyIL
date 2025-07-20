cmake_minimum_required(VERSION 3.10.0)

project(PyIL-RT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

add_subdirectory(deps)
add_library(
    ${PROJECT_NAME} SHARED

    il2cpp/utils/debug.cc    

    il2cpp/il2cpp.cc
    il2cpp/assemblies.cc
    il2cpp/state.cc

    python/sdk/sdk.cc
    python/python.cc

    entry.cc
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${Python3_LIBRARIES}
    minhook
)