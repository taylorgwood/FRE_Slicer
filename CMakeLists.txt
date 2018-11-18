cmake_minimum_required(VERSION 3.4)

project(FRE_Slicer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

IF (NOT TARGET gtest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(libs/googletest)
ENDIF()

set(TEST_NAME Slicer_UnitTest)

add_library(${PROJECT_NAME} STATIC
    shape.cpp
    shape.h
    layer.cpp
    layer.h
    path.cpp
    path.h
    point.cpp
    point.h
)

add_executable(${TEST_NAME} slicer_unittests.cpp)
target_link_libraries(${TEST_NAME} gtest gtest_main ${PROJECT_NAME})
