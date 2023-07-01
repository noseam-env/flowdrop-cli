/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#ifndef SIMPLEDEVICEINFO_HPP
#define SIMPLEDEVICEINFO_HPP

#include <optional>
#include <string>

struct SDIInfo {
    std::optional<std::string> uuid;
    std::optional<std::string> name;
    std::optional<std::string> model;
    std::optional<std::string> platform;
    std::optional<std::string> system_version;
};

void SDIFetch(SDIInfo &info);

#endif //SIMPLEDEVICEINFO_HPP
