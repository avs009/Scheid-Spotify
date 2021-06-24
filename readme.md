<p align="center">
    <img src=".github/img/logo.svg"/>
</p>

![scheid-spotify](.github/img/spotify-qt.png)

[![Linux Build](https://github.com/avs009/scheid-spotify/actions/workflows/linux.yml/badge.svg)](
https://github.com/avs009/scheid-spotify/actions/workflows/linux.yml)
[![Windows Build](https://github.com/avs009/scheid-spotify/actions/workflows/windows.yml/badge.svg)](
https://github.com/avs009/scheid-spotify/actions/workflows/windows.yml)

An unofficial Spotify client as a showcase for Qt framework.

## Supported platforms

Tested: Linux, Windows

Supported: macOS, *BSD, Haiku, any
platform [officially supported](https://en.wikipedia.org/wiki/List_of_platforms_supported_by_Qt) by
Qt

## Installing


## Building yourself

Make sure you have Qt (5.9+/6.0+) with CMake (3.5+) and various build tools like
`make`, `g++` and `git`. Keep in mind that the master branch is unstable at all times.
Therefore, it's recommended to use the stable branch, which is updated after each new stable
release, only including important bug fixes, unless you want to try out new features.

```
git clone --branch stable https://github.com/avs009/scheid-spotify.git
cd scheid-spotify
mkdir build
cd build
cmake ..
make
./ScheidSpotify
```
