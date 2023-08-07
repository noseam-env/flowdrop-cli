# FlowDrop: CLI

[FlowDrop](https://github.com/noseam-env/flowdrop) is a cross-platform tool that allows sharing files between devices directly over Wi-Fi.

The source code is published under GPL-3.0 license with anti-commercial clause, the license is available [here](https://github.com/noseam-env/flowdrop-cli/blob/master/LICENSE).


## Requirements

Local network that supports mDNS

- Windows: download and install Apple's [Bonjour](https://github.com/noseam-env/flowdrop-cli/raw/master/redist/Bonjour64.msi)
- GNU/Linux: install avahi `sudo apt-get install avahi-daemon`


## TODO

I will be glad if you help improve this project

- Improve [FlowDrop specification](https://github.com/noseam-env/flowdrop)
- Build [Bonjour](https://github.com/apple-oss-distributions/mDNSResponder) for Windows ARM.


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
Resolved
Asking receiver to accept ...
Receiver accepted
file.mp4           100%[##########################] 4 GB/4 GB
Done
```

### Receiving Files

Receiving is much easier. Simply use the `receive` command. FlowDrop will accept all incoming files automatically and put received files in the current or specified directory.
```
$ flowdrop-cli receive
```

## Build instructions

* GNU/Linux [(amd64)](docs/building-linux-amd64.md) [(arm64 cross-compile)](docs/building-linux-arm64.md)
* [macOS](docs/building-mac.md)
* [Windows](docs/building-win.md)


## Authors

- **Michael Neonov** ([email](mailto:two.nelonn@gmail.com), [github](https://github.com/Nelonn))


## Third-party

* libknotdeviceinfo ([GNU GPL v3](https://github.com/noseam-env/libknotdeviceinfo/blob/master/LICENSE))
* libflowdrop ([GNU GPL v3 with anti-commercial clause](https://github.com/noseam-env/libflowdrop/blob/master/LICENSE))
* libknotdnssd ([GNU GPL v3](https://github.com/noseam-env/libknotdnssd/blob/master/LICENSE))
* libvirtualtfa ([MIT License](https://github.com/noseam-env/libvirtualtfa/blob/master/LICENSE))
* curl 8.2.1 ([curl License](https://curl.se/docs/copyright.html))
* libhv 1.3.1 ([BSD 3-Clause License](https://github.com/ithewei/libhv/blob/v1.3.1/LICENSE))
* nlohmann_json 3.11.2 ([MIT License](https://github.com/nlohmann/json/blob/v3.11.2/LICENSE.MIT))
* Ninja ([Apache License 2.0](https://github.com/ninja-build/ninja/blob/master/COPYING))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))
