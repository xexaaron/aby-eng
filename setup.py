#!/usr/bin/env python3

"""Initial repository setup.

Run this script after cloning the repository.
After setup has completed successfully, this script can be deleted.
"""

from pathlib import Path
import subprocess


ROOT = Path(__file__).resolve().parent

def get_venv_python(venv: Path) -> Path:
    if sys.platform == "win32":
        return venv / "Scripts" / "python.exe"
    return venv / "bin" / "python"

def run(*args: str, cwd: Path = ROOT) -> None:
    subprocess.run(args, cwd=cwd, check=True)


def main() -> None:
    shaderc = ROOT / "vendor" / "aby-rhi" / "vendor" / "shaderc"
    lxqt = ROOT / "vendor" / "lxqt-build-tools"
    vendor = ROOT / "vendor"
    docs = ROOT / "aby-eng" / "docs"
    venv = docs / ".venv"

    # Setup submodules
    run("git", "submodule", "update", "--init", "--recursive", cwd=ROOT)

    # Get shaderc dependencies
    run("python", "utils/git-sync-deps", cwd=shaderc)

    # Fetch and install lxqt-build-tools for qtermwidget
    run("git", "clone", "--depth=1", "https://github.com/lxqt/lxqt-build-tools", str(lxqt), cwd=ROOT)
    run("cmake", "-S", ".", "-B", "bin", cwd=lxqt)
    run("cmake", "--build", "bin", "--target", "install", cwd=lxqt)

    # Initialize build configuration
    run("cmake", "-S", ".", "-B", "bin", cwd=ROOT)

    # Build doxide locally and install
    run("cmake", "-S", "./doxide", "-B", "bin", cwd=vendor)
    run("cmake", "--build", "bin", "--config", "release", cwd=vendor)
    run("cmake", "--install", "bin", cwd=vendor)

    # Setup .venv and get mkdocs and mkdocs-material
    run("python", "-m", "venv", ".venv", cwd=docs)
    venv_python = get_venv_python(venv)
    run(venv_python, "-m", "pip", "install", "--upgrade", "pip", "mkdocs", "mkdocs-material", cwd=docs)

if __name__ == "__main__":
    main()