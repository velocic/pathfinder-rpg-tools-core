cmake_minimum_required(VERSION 2.8)

project(PathfinderRulesEngine)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include")

add_library(
    ${PROJECT_NAME}
    SHARED
    src/abilityscores.cpp
    src/characterdescription.cpp
)

add_executable(
    ApiDemoConsumer
    src/testmain.cpp
)

target_link_libraries(ApiDemoConsumer ${PROJECT_NAME})

install(
    TARGETS
        ${PROJECT_NAME}
        ApiDemoConsumer
    DESTINATION
        bin
)
