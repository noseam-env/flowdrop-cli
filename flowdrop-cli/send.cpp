/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <vector>
#include <sstream>
#include "send.hpp"
#include "flowdrop.hpp"

std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
    std::ostringstream oss;
    auto it = vec.begin();
    if (it != vec.end()) {
        oss << *it++;
    }
    while (it != vec.end()) {
        oss << delimiter << *it++;
    }
    return oss.str();
}

void cmd_send(const std::string& receiverId, const std::vector<std::string>& files, int resolveTimeout, int acceptTimeout) {
    if (flowdrop::debug) {
        std::cout << "receiver_id: " << receiverId << std::endl;
        std::cout << "files: [" << join(files, ", ") << "]" << std::endl;
        std::cout << "resolve_timeout: " << resolveTimeout << std::endl;
        std::cout << "accept_timeout: " << acceptTimeout << std::endl;
    }

    flowdrop::send(receiverId, files, resolveTimeout, acceptTimeout);
}
