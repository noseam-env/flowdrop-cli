# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL

set(project_langs C CXX)
if (APPLE)
    set(project_langs C CXX OBJC OBJCXX)
endif()

project(simpledeviceinfo LANGUAGES ${project_langs})

set(CMAKE_CXX_STANDARD 17)

add_library(simpledeviceinfo_static STATIC src/linux.cpp src/macos.cpp src/macos_obj.mm src/windows.cpp)

target_include_directories(simpledeviceinfo_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (WIN32)
    target_link_libraries(simpledeviceinfo_static PRIVATE wbemuuid)
endif ()

find_library(IOKIT_LIBRARY IOKit)
find_library(FOUNDATION_LIBRARY Foundation)
target_link_libraries(simpledeviceinfo_static PRIVATE ${IOKIT_LIBRARY} ${FOUNDATION_LIBRARY})
