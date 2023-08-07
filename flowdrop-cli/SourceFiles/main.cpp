/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <regex>
#include <array>
#include "main.hpp"
#include "knot/deviceinfo.h"
#include "argparse/argparse.hpp"

flowdrop::DeviceInfo currentDeviceInfo = {};

bool debugEnabled = false;

std::string formatDeviceInfo(const flowdrop::DeviceInfo &info) {
    std::string result = "ID " + info.id;
    if (info.name.has_value()) {
        result += " Name=" + info.name.value();
    }
    if (info.model.has_value()) {
        result += " Model=" + info.model.value();
    }
    if (info.platform.has_value()) {
        result += " Platform=" + info.platform.value();
    }
    if (info.system_version.has_value()) {
        result += " SystemVersion=" + info.system_version.value();
    }
    return result;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("flowdrop-cli", FLOWDROP_CLI_VERSION, argparse::default_arguments::all);
    program.add_description("FlowDrop CLI");
    program.add_argument("--debug")
            .default_value(false)
            .implicit_value(true)
            .help("Show more information");

    argparse::ArgumentParser receive_command("receive", "", argparse::default_arguments::none);
    receive_command.add_description("Auto accept and receive files");
    receive_command.add_argument("-d", "--dest")
            .default_value(std::string("./"))
            .help("Destination folder for received files");
    receive_command.add_argument("--ad")
            .default_value(0)
            .scan<'i', int>()
            .help("Delay before accept file (in seconds)");
    program.add_subparser(receive_command);

    argparse::ArgumentParser find_command("find", "", argparse::default_arguments::none);
    find_command.add_description("Discover devices in local network");
    program.add_subparser(find_command);

    argparse::ArgumentParser send_command("send", "", argparse::default_arguments::none);
    send_command.add_description("Send files to receiver");
    send_command.add_argument("receiver_id")
            .help("Receiver Id")
            .required();
    send_command.add_argument("files")
            .help("Files to send")
            .nargs(argparse::nargs_pattern::at_least_one);
    send_command.add_argument("--rt")
            .default_value(10)
            .scan<'i', int>()
            .help("Resolve receiver timeout (in seconds)");
    send_command.add_argument("--at")
            .default_value(60)
            .scan<'i', int>()
            .help("Ask receiver to accept timeout (in seconds)");
    program.add_subparser(send_command);

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        if (program.is_subcommand_used("receive")) {
            std::cerr << receive_command;
        } else if (program.is_subcommand_used("find")) {
            std::cerr << find_command;
        } else if (program.is_subcommand_used("send")) {
            std::cerr << send_command;
        } else {
            std::cerr << program;
        }
        exit(EXIT_FAILURE);
    }

    if (argc == 1) {
        std::cout << program;
        return EXIT_SUCCESS;
    }

    debugEnabled = program["--debug"] == true;
    flowdrop::setDebug(debugEnabled);

    currentDeviceInfo.id = flowdrop::generate_md5_id();
    try {
        KNDeviceInfo info{};
        KNDeviceInfoFetch(info); // uuid is unused
        currentDeviceInfo.name = info.name;
        currentDeviceInfo.model = info.model;
        currentDeviceInfo.platform = info.platform;
        currentDeviceInfo.system_version = info.system_version;
    } catch (std::exception &) {
    }

    if (debugEnabled) {
        std::cout << formatDeviceInfo(currentDeviceInfo) << std::endl;
    }

    if (program.is_subcommand_used(receive_command)) {
        std::string dest = receive_command.get("--dest");
        int acceptDelay = receive_command.get<int>("--ad");
        Command::receive(dest, acceptDelay);
    }

    if (program.is_subcommand_used(find_command)) {
        Command::find();
    }

    if (program.is_subcommand_used(send_command)) {
        std::string receiverId = send_command.get("receiver_id");
        auto files = send_command.get<std::vector<std::string>>("files");
        int resolveTimeout = send_command.get<int>("--rt");
        int acceptTimeout = send_command.get<int>("--at");
        Command::send(receiverId, files, resolveTimeout, acceptTimeout);
    }

    return EXIT_SUCCESS;
}
