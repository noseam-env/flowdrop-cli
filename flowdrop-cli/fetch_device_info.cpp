/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#include "fetch_device_info.hpp"

flowdrop::DeviceInfo SimpleFetchDI(std::string id) {
    std::string uuid;
    std::string name;
    std::string model;
    std::string platform;
    std::string system_version;

#if defined(_WIN32)
    //const DWORD encodedVersion = ::GetVersion();
    //const auto majorVersion = unsigned(LOBYTE(LOWORD(encodedVersion)));
    //const auto minorVersion = unsigned(HIBYTE(LOWORD(encodedVersion)));

    platform = "Windows";

    //std::printf("Running under Windows %u.%u\n", majorVersion, minorVersion);
#elif defined(__linux__)
    std::ifstream infile("/etc/os-release");
    std::string line;

    std::unordered_map<std::string, std::string> os_info;
    while (std::getline(infile, line)) {
        auto delimiter_pos = line.find('=');
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            if (!key.empty() && !value.empty() && value.back() == '\n') {
                value.pop_back();  // Remove trailing newline
            }
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            os_info[key] = value;
        }
    }

    platform = os_info["NAME"];
    system_version = os_info["VERSION_ID"];
#elif defined(__APPLE__)
    size_t len = 0;
    sysctlbyname("kern.osrelease", NULL, &len, NULL, 0);
    char* release = (char*)malloc(len * sizeof(char));
    sysctlbyname("kern.osrelease", release, &len, NULL, 0);
    platform = "macOS";
    system_version = release;
#endif

    return {std::move(id), uuid, name, model, platform, system_version};
}
