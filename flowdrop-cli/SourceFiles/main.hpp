/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */
#pragma once

#include "flowdrop/flowdrop.hpp"

extern flowdrop::DeviceInfo currentDeviceInfo;

namespace Command {
    void receive(const std::string& dest, int acceptDelay);

    void find();

    void send(const std::string& receiverId, const std::vector<std::string>& files, int resolveTimeout, int acceptTimeout);
}
