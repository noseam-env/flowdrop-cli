# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ThirdParty/argparse)
#add_subdirectory(ThirdParty/nlohmann)

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
        send.hpp
        simple_device_info.hpp
        simple_device_info_linux.cpp
        simple_device_info_macos.cpp
        simple_device_info_windows.cpp
        )

target_include_directories(flowdrop-cli PRIVATE ../libflowdrop/include)
target_include_directories(flowdrop-cli PRIVATE ThirdParty/argparse/include)
target_include_directories(flowdrop-cli PRIVATE ThirdParty/nlohmann/include)

target_link_libraries(flowdrop-cli PRIVATE argparse)
target_link_libraries(flowdrop-cli PRIVATE libflowdrop_static)
