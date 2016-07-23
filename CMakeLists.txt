cmake_minimum_required(VERSION 2.8)
project(boostApp)

find_package(Boost 1.61.0)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not installed")
endif()

file(GLOB SOURCES "src/*.cpp")
add_executable(boostApp ${SOURCES})

target_compile_features(boostApp PRIVATE cxx_constexpr)
target_compile_features(boostApp PRIVATE cxx_generic_lambdas)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(boostApp ${Boost_LIBRARIES})

add_subdirectory(git-cmake-format)