/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <vector>
#include <sstream>
#include "main.hpp"
#include "format_util.hpp"

class SendEventListener : public flowdrop::IEventListener {
public:
    void onResolving() override {
        std::cout << "Resolving receiver ..." << std::endl;
    }

    void onReceiverNotFound() override {
        std::cout << "Receiver not found" << std::endl;
    }

    void onResolved() override {
        std::cout << "Resolved" << std::endl;
    }

    void onAskingReceiver() override {
        std::cout << "Asking receiver to accept ..." << std::endl;
    }

    void onReceiverDeclined() override {
        std::cout << "Receiver declined" << std::endl;
    }

    void onReceiverAccepted() override {
        std::cout << "Receiver accepted" << std::endl;
    }

    void onSendingStart() override {
        std::cout << "Sending files ..." << std::endl;
    }

    void onSendingFileStart(const flowdrop::FileInfo &) override {
        lastProgressTime = std::chrono::system_clock::now();
        lastProgressTime -= std::chrono::milliseconds(100);
    }

    void onSendingFileProgress(const flowdrop::FileInfo &fileInfo, std::uint64_t currentSize) override {
        std::uint64_t totalSize = fileInfo.size;
        if (totalSize == currentSize) {
            printProgress(fileInfo.name, totalSize, currentSize, true);
            return;
        }
        auto currentTime = std::chrono::system_clock::now();
        auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastProgressTime).count();
        if (elapsedMilliseconds < 100) return; // update every 0.1 sec
        printProgress(fileInfo.name, totalSize, currentSize, false);
        lastProgressTime = currentTime;
    }

    void onSendingEnd() override {
        std::cout << "Done" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> lastProgressTime;
};

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

void Command::send(const std::string& receiverId, const std::vector<std::string>& files, int resolveTimeout, int acceptTimeout) {
    if (debugEnabled) {
        std::cout << "receiver_id: " << receiverId << std::endl;
        std::cout << "files: [" << join(files, ", ") << "]" << std::endl;
        std::cout << "resolve_timeout: " << resolveTimeout << std::endl;
        std::cout << "accept_timeout: " << acceptTimeout << std::endl;
    }
    std::vector<flowdrop::File *> adaptedFiles(files.size());
    for (size_t i = 0; i < files.size(); ++i) {
        std::filesystem::path path(files[i]);
        adaptedFiles[i] = new flowdrop::NativeFile(path, path.filename().string());
    }
    auto *sendRequest = new flowdrop::SendRequest();
    sendRequest->setDeviceInfo(currentDeviceInfo);
    sendRequest->setReceiverId(receiverId);
    sendRequest->setFiles(adaptedFiles);
    sendRequest->setResolveTimeout(std::chrono::milliseconds(resolveTimeout * 1000));
    sendRequest->setAskTimeout(std::chrono::milliseconds(acceptTimeout * 1000));
    sendRequest->setEventListener(new SendEventListener());
    sendRequest->execute();
}
