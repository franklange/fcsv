cmake_minimum_required(VERSION 3.10)
project(fcsv)

add_library(fcsv INTERFACE fcsv.hpp)
target_include_directories(fcsv INTERFACE ${CMAKE_SOURCE_DIR})

add_executable(test test/test.cpp)

target_link_libraries(test PRIVATE fcsv)
target_compile_features(test PRIVATE cxx_std_17)
target_compile_options(test PRIVATE -O3)

configure_file(${CMAKE_SOURCE_DIR}/test/test.csv ${CMAKE_BINARY_DIR}/test.csv COPYONLY)