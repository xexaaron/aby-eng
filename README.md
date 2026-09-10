# README - aby-eng

## Project Setup

### Requirements

- [![Python](https://img.shields.io/badge/Python-3.14.7-3776AB?logo=python&logoColor=fff)](#)
- [![CMake](https://img.shields.io/badge/CMake-%3E%3D3.28.3-064F8C?logo=CMake&logoColor=white)](#)
- [![Vulkan](https://img.shields.io/badge/Vulkan-1.4-AE0F28?logo=Vulkan&logoColor=fff)](#)

### Cloning

```shell
git clone --depth=1 --recurse-submodules https://github.com/xexaaron/aby-eng.git
```

### Setup

#### [![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#)
```shell
cd aby-eng
chmod +x ./setup.sh
./setup.sh
```
#### [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#)

```shell
cd aby-eng
./setup.bat
```
#### Building

``` shell
cmake --build bin --config <Release|Debug|RelWithDebInfo>
```
