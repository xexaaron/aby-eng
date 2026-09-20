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
    run("git", "submodule", "update", "--init", "--recursive")

    shaderc = ROOT / "vendor" / "aby-rhi" / "vendor" / "shaderc"
    run("python", "utils/git-sync-deps.py", cwd=shaderc)

    run("cmake", "-S", ".", "-B", "bin")


if __name__ == "__main__":
    main()