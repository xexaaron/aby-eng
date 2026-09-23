#!/usr/bin/env python3

"""Initial repository setup.

Run this script after cloning the repository.
After setup has completed successfully, this script can be deleted.
"""

from pathlib import Path
import subprocess


ROOT = Path(__file__).resolve().parent

def run(*args: str, cwd: Path = ROOT) -> None:
    subprocess.run(args, cwd=cwd, check=True)


def main() -> None:
    # Setup submodules
    run("git", "submodule", "update", "--init", "--recursive", cwd=ROOT)

    # Get shaderc dependencies
    shaderc = ROOT / "vendor" / "aby-rhi" / "vendor" / "shaderc"
    run("python", "utils/git-sync-deps", cwd=shaderc)

    # Fetch and install lxqt-build-tools for qtermwidget
    lxqt = ROOT / "vendor" / "lxqt-build-tools"
    run("git", "clone", "--depth=1", "https://github.com/lxqt/lxqt-build-tools", str(lxqt), cwd=ROOT)
    run("cmake", "-S", ".", "-B", "bin", cwd=lxqt)
    run("cmake", "--build", "bin", "--target", "install", cwd=lxqt)

    # Initialize build configuration
    run("cmake", "-S", ".", "-B", "bin", cwd=ROOT)


if __name__ == "__main__":
    main()