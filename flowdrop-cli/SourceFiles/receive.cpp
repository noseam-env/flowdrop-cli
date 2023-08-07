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

    void onReceivingStart(const flowdrop::DeviceInfo &sender, std::uint64_t totalSize) override {
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

    void onReceivingFileProgress(const flowdrop::DeviceInfo &, const flowdrop::FileInfo &fileInfo, std::uint64_t receivedSize) override {
        std::uint64_t totalSize = fileInfo.size;
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

    void onReceivingEnd(const flowdrop::DeviceInfo &sender, std::uint64_t totalSize, const std::vector<flowdrop::FileInfo> &receivedFiles) override {
        std::cout << "Receiving done" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> lastProgressTime;
};

flowdrop::Server *server = nullptr;

void stop([[maybe_unused]] int signal) {
    // TODO: hide message "poll: No error"
    std::ios_base::sync_with_stdio(false);
    std::cout.setf(std::ios_base::unitbuf);
    if (server) {
        server->stop();
    }
    std::exit(0);
}

void Command::receive(const std::string &dest, int acceptDelay) {
    std::signal(SIGINT, stop);

    if (debugEnabled) {
        std::cout << "dest: " << dest << std::endl;
        std::cout << "accept_delay: " << std::to_string(acceptDelay) << std::endl;
    }

    server = new flowdrop::Server(currentDeviceInfo);
    server->setEventListener(new ReceiveEventListener());
    server->setAskCallback([&acceptDelay](const flowdrop::SendAsk &sendAsk){
        if (acceptDelay > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(acceptDelay));
        }
        return true;
    });
    server->setDestDir(dest);
    server->run();
}
