#!/bin/bash

# Run this script after cloning the repository for initial setup.
# After this has been run you can safely delete it.

set -e

ROOT="$(cd "$(dirname "$0")" && pwd)"

cd "$ROOT"

git submodule update --init --recursive

cd "$ROOT/vendor/aby-rhi/vendor/shaderc"
py utils/git-sync-deps.py

cd "$ROOT"

cmake -S . -B bin