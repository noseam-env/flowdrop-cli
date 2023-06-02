/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include "find.hpp"
#include "flowdrop.hpp"

void callback(const flowdrop::Receiver& receiver) {
    flowdrop::DeviceInfo deviceInfo = receiver.deviceInfo;
    std::cout << "Found: ID " << deviceInfo.id;
    if (!deviceInfo.name.empty()) {
        std::cout << " Name=" << deviceInfo.name;
    }
    if (!deviceInfo.model.empty()) {
        std::cout << " Model=" << deviceInfo.model;
    }
    if (!deviceInfo.platform.empty()) {
        std::cout << " Platform=" << deviceInfo.platform;
    }
    if (!deviceInfo.system_version.empty()) {
        std::cout << " SystemVersion=" << deviceInfo.system_version;
    }
    std::cout << std::endl;
}

void cmd_find() {
    std::cout << "Looking for receivers. Press Ctrl+C to stop ..." << std::endl;
    flowdrop::find(callback);
}
