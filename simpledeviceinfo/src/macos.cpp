/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#if defined(__APPLE__)

#include "simpledeviceinfo.hpp"
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include <string>
#include <locale>
#include <codecvt>

extern "C" {
const char* getCurrentHostNameC();
}

std::string getUUID() {
    std::string uuid;

    io_service_t platformExpert = IOServiceGetMatchingService(kIOMasterPortDefault,
                                                              IOServiceMatching("IOPlatformExpertDevice"));
    if (platformExpert) {
        CFTypeRef uuidProperty = IORegistryEntryCreateCFProperty(platformExpert, CFSTR(kIOPlatformUUIDKey),
                                                                 kCFAllocatorDefault, 0);
        if (uuidProperty) {
            auto uuidStringRef = static_cast<CFStringRef>(uuidProperty);
            char buffer[256];
            if (CFStringGetCString(uuidStringRef, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
                uuid = buffer;
            }
            CFRelease(uuidProperty);
        }
        IOObjectRelease(platformExpert);
    }

    std::transform(uuid.begin(), uuid.end(), uuid.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return uuid;
}

std::string getVersion() {
    std::string version;
    FILE *pipe = popen("sw_vers -productVersion", "r");
    if (pipe != nullptr) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            version = buffer;
            version.erase(version.find_last_not_of('\n') + 1);
        }
        pclose(pipe);
    }
    return version;
}

std::string convertToUTF8(const char* str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideStr = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str);
    return converter.to_bytes(wideStr);
}

void SDIFetch(SDIInfo &info) {
    info.uuid = getUUID();
    info.name = convertToUTF8(getCurrentHostNameC());
    info.platform = "macOS";
    try {
        info.system_version = getVersion();
    } catch (const std::exception &) {
    }
}

#endif
