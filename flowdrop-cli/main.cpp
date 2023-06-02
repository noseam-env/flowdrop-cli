/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#include <iostream>
#include <regex>
#include <array>

#include "flowdrop.hpp"
#include "argparse/argparse.hpp"
#include "receive.hpp"
#include "find.hpp"
#include "send.hpp"
#include "fetch_device_info.hpp"

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("flowdrop-cli", "0.0.1", argparse::default_arguments::help);
    program.add_description("FlowDrop CLI");
    program.add_argument("--debug").default_value(false).implicit_value(true).help("Show more information");

    argparse::ArgumentParser receive_command("receive", "", argparse::default_arguments::none);
    receive_command.add_description("Auto accept and receive files");
    receive_command.add_argument("-d", "--dest").default_value(std::string("./")).help("Destination folder for received files");
    receive_command.add_argument("--ad").default_value(0).scan<'i', int>().help("Delay before accept file (in seconds)");
    program.add_subparser(receive_command);

    argparse::ArgumentParser find_command("find", "", argparse::default_arguments::none);
    find_command.add_description("Lookup for devices in local network");
    program.add_subparser(find_command);

    argparse::ArgumentParser send_command("send", "", argparse::default_arguments::none);
    send_command.add_description("Send files to receiver");
    send_command.add_argument("receiver_id").help("Receiver Id").required();
    send_command.add_argument("files").help("Files to send").nargs(argparse::nargs_pattern::at_least_one);
    send_command.add_argument("--rt").default_value(10).scan<'i', int>().help("Resolve receiver timeout (in seconds)");
    send_command.add_argument("--at").default_value(60).scan<'i', int>().help("Ask receiver to accept timeout (in seconds)");
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

    flowdrop::debug = program["--debug"] == true;

    flowdrop::thisDeviceInfo = SimpleFetchDI(flowdrop::gen_md5_id());

    if (flowdrop::debug) {
        json j = flowdrop::thisDeviceInfo;
        std::string json_str = j.dump();
        std::cout << json_str << std::endl;
    }

    if (program.is_subcommand_used(receive_command)) {
        std::string dest = receive_command.get("--dest");
        int acceptDelay = receive_command.get<int>("--ad");
        cmd_receive(dest, acceptDelay);
    }

    if (program.is_subcommand_used(find_command)) {
        cmd_find();
    }

    if (program.is_subcommand_used(send_command)) {
        std::string receiverId = send_command.get("receiver_id");
        auto files = send_command.get<std::vector<std::string>>("files");
        int resolveTimeout = send_command.get<int>("--rt");
        int acceptTimeout = send_command.get<int>("--at");
        cmd_send(receiverId, files, resolveTimeout, acceptTimeout);
    }

    return EXIT_SUCCESS;
}
