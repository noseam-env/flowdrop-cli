/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#if defined(__linux__)

#include "simple_device_info.hpp"
#include <fstream>

void SimpleDeviceInfo(flowdrop::DeviceInfo &deviceInfo) {
    std::ifstream infile("/etc/os-release");
    std::string line;

    std::unordered_map<std::string, std::string> os_info;
    while (std::getline(infile, line)) {
        auto delimiter_pos = line.find('=');
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            if (!key.empty() && !value.empty() && value.back() == '\n') {
                value.pop_back();  // Remove trailing newline
            }
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            os_info[key] = value;
        }
    }

    deviceInfo.platform = os_info["NAME"];
    deviceInfo.system_version = os_info["VERSION_ID"];
}

#endif
