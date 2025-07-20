cmake_minimum_required(VERSION 3.10.0)

project(PyIL-Injector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME}
    main.cc
)