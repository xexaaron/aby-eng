# README - aby-eng

## Project Setup

| Requirement                                                                                                   | Version                                                            | Platform |
| ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | -------- |
| [![Python](https://img.shields.io/badge/Python-3776AB?logo=python\&logoColor=fff)](#)                         | ![Version](https://img.shields.io/badge/version-%3E=_3.14.17-blue) | ![Platform](https://img.shields.io/badge/Cross-Platform-555)        |
| [![CMake](https://img.shields.io/badge/CMake-064F8C?logo=CMake\&logoColor=fff)](#)                            | ![Version](https://img.shields.io/badge/version-%3E=_3.28.3-blue)  | ![Platform](https://img.shields.io/badge/Cross-Platform-555)        |
| [![Vulkan](https://img.shields.io/badge/Vulkan-AE0F28?logo=Vulkan\&logoColor=fff)](#)                         | ![Version](https://img.shields.io/badge/version-%3E=_1.4.341-blue) | ![Platform](https://img.shields.io/badge/Cross-Platform-555)        |
| [![Qt](https://img.shields.io/badge/Qt-2CDE85?logo=Qt\&logoColor=fff)](#)                                     | ![Version](https://img.shields.io/badge/version-6-blue)            | ![Platform](https://img.shields.io/badge/Cross-Platform-555)      |
| [![MSVC Toolchain](https://img.shields.io/badge/MSVC%20Toolchain-5C2D91?logo=visualstudio\&logoColor=fff)](#) | ![Version](https://img.shields.io/badge/version-2026-blue)         | [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#)  |


### Cloning

```shell
git clone --depth=1 --recurse-submodules https://github.com/xexaaron/aby-eng
cd aby-eng
python3 ./setup.py
```

### Building

``` shell
cmake --build bin --config <Release|Debug|RelWithDebInfo>
```
