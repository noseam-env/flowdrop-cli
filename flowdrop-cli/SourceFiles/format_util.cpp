/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/libflowdrop/blob/master/LEGAL
 */

#include "format_util.hpp"
#include "os_util.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>


std::string adjustStringToLength(const std::string& input, size_t length) {
    if (input.length() < length) {
        return input + std::string(length - input.length(), ' ');
    } else if (input.length() > length) {
        return input.substr(0, length);
    }
    return input;
}

constexpr int base = 1024;
constexpr const char* suffixes[] = {"B", "KB", "MB", "GB"};

std::string formatBytes(std::uint64_t bytes) {
    if (bytes < base) {
        return std::to_string(bytes) + " " + suffixes[0];
    }

    int exp = static_cast<int>(std::log(bytes) / std::log(base));
    double val = static_cast<double>(bytes) / std::pow(base, exp);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << val;

    return ss.str() + " " + suffixes[exp];
}

#define PROGRESS_BAR_LENGTH 30

constexpr int progressBarRatio = 100 / PROGRESS_BAR_LENGTH;

void printProgress(const std::string& fileName, std::uint64_t totalSize, std::uint64_t currentSize, bool last) {
    int progress = static_cast<int>((static_cast<double>(currentSize) / static_cast<double>(totalSize)) * 100);

    std::string progressBar;
    progressBar.reserve(PROGRESS_BAR_LENGTH);
    int numBars = std::min(PROGRESS_BAR_LENGTH, progress / progressBarRatio);
    progressBar.append(numBars, '#');
    progressBar.append(PROGRESS_BAR_LENGTH - numBars, ' ');

    std::ostringstream progressNum;
    progressNum << std::setw(3) << std::setfill(' ') << progress;

    std::string line = adjustStringToLength(fileName, 18) + " " + progressNum.str() + "%[" +
                       progressBar + "] " + formatBytes(currentSize) + "/" + formatBytes(totalSize);

    std::cout << "\r" << line << std::setw(static_cast<std::streamsize>(getConsoleWidth()) - static_cast<std::streamsize>(line.length())) << " ";
    if (last) {
        std::cout << std::endl;
    }
    std::cout.flush();
}
