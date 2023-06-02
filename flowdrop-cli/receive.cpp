/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <filesystem>
#include <thread>
#include "receive.hpp"
#include "flowdrop.hpp"

void cmd_receive(const std::string &dest, int acceptDelay) {
    if (flowdrop::debug) {
        std::cout << "dest: " << dest << std::endl;
        std::cout << "accept_delay: " << std::to_string(acceptDelay) << std::endl;
    }

    flowdrop::receive(dest, [&acceptDelay](const flowdrop::SendAsk &sendAsk){
        if (acceptDelay > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(acceptDelay));
        }
        return true;
    });
}
