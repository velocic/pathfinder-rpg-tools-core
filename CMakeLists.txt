cmake_minimum_required(VERSION 2.8)

project(PathfinderRulesEngine)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")
include_directories("include" "tests/include")

if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
endif(UNIX AND NOT APPLE)


add_library(
    ${PROJECT_NAME}
    SHARED
    src/abilityscores.cpp
    src/characterdescription.cpp
    src/baseattackbonus.cpp
)

add_executable(
    ApiDemoConsumer
    src/testmain.cpp
)

add_executable(
    Tests
    tests/abilityscoretests.cpp
    tests/baseattackbonustests.cpp
)

target_link_libraries(ApiDemoConsumer ${PROJECT_NAME})

#If Linux or Apple, link libs on the system path (except ${PROJECT_NAME}, which will be in the build directory next to the target
#But if we're on windows, find the libs (and their headers) in the dependencies dir local to our project, since windows has no standard library path
if (LINUX OR APPLE)
    target_link_libraries(Tests ${PROJECT_NAME} gtest gtest_main pthread)
else()
    include_directories("dependencies/include")
    target_link_libraries(
        Tests
        ${PROJECT_NAME}
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/gtest.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/gtest_main.lib"
    )
endif(LINUX OR APPLE)
