/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include "main.hpp"

void callback(const flowdrop::DeviceInfo& deviceInfo) {
    std::cout << "Found: ID " << deviceInfo.id;
    if (deviceInfo.name.has_value()) {
        std::cout << " Name=" << deviceInfo.name.value();
    }
    if (deviceInfo.model.has_value()) {
        std::cout << " Model=" << deviceInfo.model.value();
    }
    if (deviceInfo.platform.has_value()) {
        std::cout << " Platform=" << deviceInfo.platform.value();
    }
    if (deviceInfo.system_version.has_value()) {
        std::cout << " SystemVersion=" << deviceInfo.system_version.value();
    }
    std::cout << std::endl;
}

void Command::find() {
    std::cout << "Looking for receivers. Press Ctrl+C to stop ..." << std::endl;
    flowdrop::find(callback);
}
