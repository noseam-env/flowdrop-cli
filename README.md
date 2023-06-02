# FlowDrop: CLI Implementation

[FlowDrop](https://github.com/Nelonn/flowdrop) is a cross-platform tool that allows sharing files between devices directly over Wi-Fi.

On Windows download and install Apple's [Bonjour SDK for Windows v3.0](https://raw.githubusercontent.com/FlowDrop/libflowdrop/master/redist/bonjoursdksetup.exe)

The source code is published under GPL-3.0 license with anti-commercial clause, the license is available [here](https://github.com/FlowDrop/flowdrop-cli/blob/master/LICENSE).

#### OS compatibility

- windows AMD64: `working` *(requires Bonjour SDK)*
- Windows ARM64: `requires to build Bonjour SDK`
- Linux: `requires a few fixes and libavahi-compat-libdnssd-dev` *(avahi)*
- macOS: `not tested, probably working`


## Requirements

Local network that supports mDNS

On Windows download and install Apple's [Bonjour SDK for Windows v3.0](https://raw.githubusercontent.com/Nelonn/flowdrop-cli/master/redist/bonjoursdksetup.exe)


## TODO

I will be glad if you help improve this project

- All TODO from [FlowDrop specification](https://github.com/FlowDrop/flowdrop)
- Sender/Receiver authentication and connection state.
- Build [Bonjour SDK Open Source](https://github.com/apple-oss-distributions/mDNSResponder) for arm64.


## Usage

I briefly explain how to send and receive files using `flowdrop-cli`. To see all command line options, run `flowdrop-cli -h`.

### Sending a File

Sending a file is typically a two-step procedure. You first discover devices in local network using the `find` command.
Stop the process once you have found the receiver.
```
$ flowdrop-cli find
Looking for receivers. Press Ctrl+C to stop ...
Found: ID f56648ba9f79 Name=NelonnPC Model=MS-7C56 Platform=Windows SystemVersion=11
Found: ID 99e1e5bd7100 Name=NelonnPhone Model=Pixel 7 Platform=Android SystemVersion=13
```
You can then `send` a file using
```
$ flowdrop-cli send f56648ba9f79 path/to/some/file.mp4
Resolving receiver ...
Resolved TestPC.local.:39979
Asking receiver to accept ...
Receiver accepted
file.mp4                   100%[##########################] 4 GB/4 GB | 64 MB/s
Done!
```

### Receiving Files

Receiving is much easier. Simply use the `receive` command. FlowDrop will accept all incoming files automatically and put received files in the current or specified directory.
```
$ flowdrop-cli receive
```


## Authors

- **Michael Neonov** ([email](mailto:two.nelonn@gmail.com), [github](https://github.com/Nelonn))


## Third-party

* asio 1.28.0 ([Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt))
* bzip2 1.0.8 ([bzip2 License](https://gitlab.com/bzip2/bzip2/-/blob/bzip2-1.0.8/LICENSE))
* curl 8.1.1 ([curl License](https://curl.se/docs/copyright.html))
* libarchive 3.6.2 ([New BSD License](https://raw.githubusercontent.com/libarchive/libarchive/master/COPYING))
* libhv 1.3.1 ([BSD 3-Clause License](https://github.com/ithewei/libhv/blob/v1.3.1/LICENSE))
* nlohmann_json 3.11.2 ([MIT License](https://github.com/nlohmann/json/blob/v3.11.2/LICENSE.MIT))
* liblzma 5.4.3 ([public domain](http://tukaani.org/xz/))
* zlib 1.2.11 ([zlib License](http://www.zlib.net/zlib_license.html))
* zstd 1.5.5 ([BSD License](https://github.com/facebook/zstd/blob/v1.5.5/LICENSE))
* Ninja ([Apache License 2.0](https://github.com/ninja-build/ninja/blob/master/COPYING))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))
