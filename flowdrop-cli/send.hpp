/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#ifndef FLOWDROP_CLI_SEND_HPP
#define FLOWDROP_CLI_SEND_HPP


void cmd_send(const std::string& receiverId, const std::vector<std::string>& files, int resolveTimeout, int acceptTimeout);


#endif //FLOWDROP_CLI_SEND_HPP
