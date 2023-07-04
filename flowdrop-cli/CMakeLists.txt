# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ThirdParty/argparse)
add_subdirectory(ThirdParty/libflowdrop)
add_subdirectory(ThirdParty/libknotdeviceinfo)

add_executable(flowdrop-cli
        find.cpp
        find.hpp
        format_util.cpp
        format_util.hpp
        main.cpp
        main.hpp
        receive.cpp
        receive.hpp
        send.cpp
        send.hpp)

target_link_libraries(flowdrop-cli PRIVATE
        nlohmann_json::nlohmann_json
        knotdeviceinfo_static
        libflowdrop_static
        argparse)
