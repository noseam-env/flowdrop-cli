# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
cmake_policy(SET CMP0111 NEW)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:ucrt.lib")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(flowdrop-cli)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(third_party_loc "flowdrop-cli/ThirdParty" REALPATH)
get_filename_component(submodules_loc "flowdrop-cli" REALPATH)

add_subdirectory(libflowdrop)
add_subdirectory(flowdrop-cli)
