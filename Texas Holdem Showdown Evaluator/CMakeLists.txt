cmake_minimum_required(VERSION 3.10)
project(GenericCfrLib VERSION 1.0)
set(CMAKE_CXX_STANDARD 23)

set(LIB_SOURCES
        src/CardBitMask.cpp
        src/EvaluatorClass.cpp
        EvaluatorLibrary.cpp
        src/HandBitMask.cpp
        src/dllmain.cpp
        src/pch.cpp
)

add_library(HoldEmEvaluator SHARED ${LIB_SOURCES})

target_include_directories(GenericCfrLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
)