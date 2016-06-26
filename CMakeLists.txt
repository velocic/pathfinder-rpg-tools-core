cmake_minimum_required(VERSION 2.8)

project(PathfinderRulesEngine)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
endif()

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

add_executable(
    Tests
    tests/abilityscoretests.cpp
)

target_link_libraries(ApiDemoConsumer ${PROJECT_NAME})

if (LINUX)
    target_link_libraries(Tests ${PROJECT_NAME} gtest gtest_main pthread)
else()
    target_link_libraries(Tests ${PROJECT_NAME} gtest gtest_main)
endif()

# Leaving this out for now. Thought bin would be relative to the project path, but
# it wants to install into /usr/local
# install(
#     TARGETS
#         ${PROJECT_NAME}
#         ApiDemoConsumer
#         Tests
#     DESTINATION
#         bin
# )
