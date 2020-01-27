cmake_minimum_required(VERSION 3.5)
PROJECT(RXS_Parsing_Core)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")

file(GLOB rxs_parsing_source *.*)
ADD_LIBRARY(rxs_parsing SHARED ${rxs_parsing_source})
add_subdirectory(test)