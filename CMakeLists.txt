cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    r-type
    VERSION 0.1
    DESCRIPTION "Make a multiplayer r-type game"
    LANGUAGES CXX
)

if (WIN32)
    include(${PROJECT_SOURCE_DIR}/build/conanbuildinfo.cmake)
endif ()

if (UNIX)
    include(${PROJECT_SOURCE_DIR}/conanbuildinfo.cmake)
endif()

conan_basic_setup()

Set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST -D_WIN32_WINNT=0x0601 -DBOOST_BIND_GLOBAL_PLACEHOLDERS" )
ENDIF()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/client/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/server/CMakeLists.txt)