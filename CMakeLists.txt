# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0111 NEW)

include(cmake/version.cmake)
app_parse_version(flowdrop-cli/version)

project(flowdrop-cli
        LANGUAGES CXX
        VERSION ${app_version_cmake})

add_subdirectory(flowdrop-cli)
