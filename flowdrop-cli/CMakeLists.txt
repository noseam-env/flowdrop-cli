# This file is part of flowdrop-cli.
#
# For license and copyright information please follow this link:
# https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    option(BUILD_FOR_MT "" ON)
    set(BUILD_FOR_MT ON)
    add_compile_options(
            $<$<CONFIG:>:/MT> #---------|
            $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
            $<$<CONFIG:Release>:/MT> #--|
    )
endif ()

add_subdirectory(ThirdParty/argparse)
add_subdirectory(ThirdParty/libflowdrop)
add_subdirectory(ThirdParty/libknotdeviceinfo)

add_definitions(-DFLOWDROP_CLI_VERSION="${app_version}")

set(product_company_name "Michael Neonov")
set(product_company_copyright "Michael Neonov (C) Copyright ${CURRENT_YEAR}")
set(product_name "FlowDrop CLI")
set(product_internal_name "flowdrop-cli")
set(product_original_filename "flowdrop-cli.exe")
set(product_file_description ${product_name})
set(product_comments ${product_name})

set(FWC_SOURCES
        SourceFiles/find.cpp
        SourceFiles/format_util.cpp
        SourceFiles/format_util.hpp
        SourceFiles/main.cpp
        SourceFiles/main.hpp
        SourceFiles/os_util.cpp
        SourceFiles/os_util.h
        SourceFiles/receive.cpp
        SourceFiles/send.cpp)

if (WIN32)
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/Resources/app.rc.in
            ${CMAKE_CURRENT_BINARY_DIR}/Resources/app.rc
            @ONLY)
    set(FWC_SOURCES ${FWC_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/Resources/app.rc)
endif ()

add_executable(flowdrop-cli ${FWC_SOURCES})

set_target_properties(flowdrop-cli PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(flowdrop-cli PRIVATE
        knotdeviceinfo_static
        libflowdrop_static
        argparse)
