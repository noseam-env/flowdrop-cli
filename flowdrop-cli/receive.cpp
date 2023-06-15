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

class ReceiveEventListener : public flowdrop::IEventListener {
public:
    void onReceiverStarted(unsigned short port) override {
        std::cout << "Receiving as " << flowdrop::thisDeviceInfo.id << std::endl;
        std::cout << "Press Ctrl+C to stop ..." << std::endl;
    }

    void onReceivingStart(const flowdrop::DeviceInfo &sender, std::size_t totalSize) override {
        std::cout << "Receiving files ..." << std::endl;
    }

    void onReceivingEnd(const flowdrop::DeviceInfo &sender, std::size_t totalSize) override {
        std::cout << "Receiving done" << std::endl;
    }
};

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
    }, new ReceiveEventListener());
}
