# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ThirdParty/argparse)
add_subdirectory(ThirdParty/libflowdrop)
add_subdirectory(ThirdParty/libknotdeviceinfo)

add_definitions(-DFLOWDROP_CLI_VERSION="${app_version}")

add_executable(flowdrop-cli
        SourceFiles/find.cpp
        SourceFiles/format_util.cpp
        SourceFiles/format_util.hpp
        SourceFiles/main.cpp
        SourceFiles/main.hpp
        SourceFiles/receive.cpp
        SourceFiles/send.cpp SourceFiles/os_util.cpp SourceFiles/os_util.h)

target_link_libraries(flowdrop-cli PRIVATE
        knotdeviceinfo_static
        libflowdrop_static
        argparse)
