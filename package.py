import argparse
import platform
import shutil
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Package an Abyss Engine build."
    )

    system = {
        "Windows": "win32",
        "Linux": "linux",
        "Darwin": "macos",
    }[platform.system()]

    parser.add_argument(
        "--build",
        default="Debug",
        help="Build configuration (default: Debug)",
    )

    parser.add_argument(
        "--output",
        default=".",
        help="Output directory (default: .)",
    )

    parser.add_argument(
        "--version",
        default="1.0",
        help="Package version (default: 1.0)",
    )

    args = parser.parse_args()

    arch = platform.machine()
    build_dir = Path("bin") / args.build

    if not build_dir.is_dir():
        raise FileNotFoundError(
            f"Build directory does not exist: {build_dir}"
        )

    output_dir = Path(args.output)

    package_dir = (
        output_dir /
        f"aby-eng-{args.version}-{system}-{arch}"
    )

    package_dir.mkdir(parents=True, exist_ok=True)

    # Copy Abyss Engine executables and other extensionless files.
    for path in build_dir.iterdir():
        if not path.is_file():
            continue

        # Don't package static libraries or shared libraries here.
        if path.suffix in {".a", ".so", ".dll", ".exe"}:
            continue

        # Don't package SPIR-V/shaderc tools.
        if path.name.startswith("spirv-"):
            continue

        shutil.copy2(
            path,
            package_dir / path.name,
        )

    # Copy platform-specific binaries.
    for extension in (".exe", ".so", ".dll"):
        for path in build_dir.glob(f"*{extension}"):
            if path.is_file():
                shutil.copy2(
                    path,
                    package_dir / path.name,
                )

    # Copy resources.
    resource_dir = build_dir / "resource"

    if resource_dir.is_dir():
        shutil.copytree(
            resource_dir,
            package_dir / "resource",
            dirs_exist_ok=True,
        )

    print(f"Packaged: {package_dir}")


if __name__ == "__main__":
    main()