/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/libflowdrop/blob/master/LEGAL
 */

#include <string>
#include <cmath>
#include <sstream>
#include "format_util.hpp"

std::string formatBytes(size_t bytes) {
    const int base = 1024;
    const char* suffixes[] = {"B", "KB", "MB", "GB"};

    if (bytes < base) {
        return std::to_string(bytes) + " " + suffixes[0];
    }

    int exp = static_cast<int>(std::log(bytes) / std::log(base));
    double val = static_cast<double>(bytes) / std::pow(base, exp);
    std::stringstream ss;
    ss << val;
    std::string formattedVal = ss.str();
    formattedVal = formattedVal.substr(0, formattedVal.find('.') + 2);

    return formattedVal + " " + suffixes[exp];
}
