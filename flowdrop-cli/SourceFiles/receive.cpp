/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <csignal>
#include <thread>
#include "main.hpp"
#include "format_util.hpp"

class ReceiveEventListener : public flowdrop::IEventListener {
public:
    void onReceiverStarted(unsigned short port) override {
        std::cout << "Receiving as " << currentDeviceInfo.id << std::endl;
        std::cout << "Press Ctrl+C to stop ..." << std::endl;
    }

    void onReceivingStart(const flowdrop::DeviceInfo &sender, std::size_t totalSize) override {
        std::cout << "Receiving files from " << sender.id;
        if (sender.name.has_value()) {
            std::cout << " " << sender.name.value();
        }
        std::cout << " ..." << std::endl;
    }

    void onReceivingFileStart(const flowdrop::DeviceInfo &, const flowdrop::FileInfo &) override {
        lastProgressTime = std::chrono::system_clock::now();
        lastProgressTime -= std::chrono::milliseconds(100);
    }

    void onReceivingFileProgress(const flowdrop::DeviceInfo &, const flowdrop::FileInfo &fileInfo, std::size_t receivedSize) override {
        std::size_t totalSize = fileInfo.size;
        if (totalSize == receivedSize) {
            printProgress(fileInfo.name, totalSize, receivedSize, true);
            return;
        }
        auto currentTime = std::chrono::system_clock::now();
        auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastProgressTime).count();
        if (elapsedMilliseconds < 100) return; // update every 0.1 sec
        printProgress(fileInfo.name, totalSize, receivedSize, false);
        lastProgressTime = currentTime;
    }

    void onReceivingEnd(const flowdrop::DeviceInfo &sender, std::size_t totalSize) override {
        std::cout << "Receiving done" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> lastProgressTime;
};

flowdrop::Receiver *receiver = nullptr;

void stop([[maybe_unused]] int signal) {
    // TODO: hide message "poll: No error"
    std::ios_base::sync_with_stdio(false);
    std::cout.setf(std::ios_base::unitbuf);
    if (receiver) {
        receiver->stop();
    }
    std::exit(0);
}

void Command::receive(const std::string &dest, int acceptDelay) {
    std::signal(SIGINT, stop);

    if (flowdrop::debug) {
        std::cout << "dest: " << dest << std::endl;
        std::cout << "accept_delay: " << std::to_string(acceptDelay) << std::endl;
    }

    receiver = new flowdrop::Receiver(currentDeviceInfo);
    receiver->setEventListener(new ReceiveEventListener());
    receiver->setAskCallback([&acceptDelay](const flowdrop::SendAsk &sendAsk){
        if (acceptDelay > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(acceptDelay));
        }
        return true;
    });
    receiver->setDestDir(dest);
    receiver->run();
}
