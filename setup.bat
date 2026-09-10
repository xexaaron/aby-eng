@echo off
setlocal

REM Run this script after cloning the repository for initial setup.
REM After this has been run you can safely delete it.

cd /d "%~dp0"

git submodule update --init --recursive

cd /d "%~dp0vendor\aby-rhi\vendor\shaderc"

py utils\git-sync-deps.py

cd /d "%~dp0"

cmake -S . -B bin

endlocal