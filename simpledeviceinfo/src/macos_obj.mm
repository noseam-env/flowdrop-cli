#import <Foundation/Foundation.h>

extern "C" {
const char* getCurrentHostNameC();
}

NSString *getCurrentHostName() {
    NSHost *currentHost = [NSHost currentHost];
    NSString *localizedName = [currentHost localizedName];
    return localizedName;
}

const char* getCurrentHostNameC() {
    NSString *hostName = getCurrentHostName();
    const char *cString = [hostName cStringUsingEncoding:NSUTF8StringEncoding];
    return cString;
}
