cmake_minimum_required(VERSION 3.5)

#
# Project configuration
#

project(DevTest)

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Warnings that are globally disabled (and why):
    # 4201 - Disable Nameless struct / union (glm)
    # N.B - /Za would also fix these warnings internally within glm but this breaks gtest and various
    #       windows headers
    # 4503 - 'identifier' : decorated name length exceeded, name was truncated
    # 4100 - unreferenced formal parameter
    #
    # Other compiler flags:
    # /MP -> Multiprocess builds
    # /WX -> Treat compiler warnings as errors
    # _CRT_SECURE_NO_WARNINGS -> Disable deprectation of things like fopen, sprintf, etc
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4201 /wd4503 /wd4100 /MP /WX /D_CRT_SECURE_NO_WARNINGS=1")
else()
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

#
# RPATH settings
#

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

if (APPLE)
    list(APPEND CMAKE_INSTALL_RPATH "@executable_path/../lib")
else ()
    list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")
endif ()

#
# Define the main DevTest build
#

add_library(DevTest STATIC
    # External
    catch.hpp

    # headers
    Deserializer.h
    Serializer.h
    Structures.h

    # sources
    Deserializer.cc
    Serializer.cc
    Structures.cc
)

set_property(TARGET DevTest PROPERTY CXX_STANDARD 14)


#
# DevTest tests
#
enable_testing()

add_executable(DevTestTests
    Tests.cc
)

target_link_libraries(DevTestTests DevTest)

add_test(DevTestTests DevTestTests)

set_property(TARGET DevTestTests PROPERTY CXX_STANDARD 14)
