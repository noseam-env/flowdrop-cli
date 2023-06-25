/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#if defined(__APPLE__)

#include "simple_device_info.hpp"

void SimpleDeviceInfo(flowdrop::DeviceInfo &deviceInfo) {
    size_t len = 0;
    sysctlbyname("kern.osrelease", NULL, &len, NULL, 0);
    char* release = (char*)malloc(len * sizeof(char));
    sysctlbyname("kern.osrelease", release, &len, NULL, 0);
    deviceInfo.platform = "macOS";
    deviceInfo.system_version = release;
}

#endif
