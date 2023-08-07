/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include "main.hpp"

void callback(const flowdrop::DeviceInfo& deviceInfo) {
    std::cout << formatDeviceInfo(deviceInfo) << std::endl;
}

void Command::find() {
    std::cout << "Looking for receivers. Press Ctrl+C to stop ..." << std::endl;
    flowdrop::discover(callback);
}
