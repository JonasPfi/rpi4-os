# Teardrop-OS

# Build Instructions

This project uses a Makefile to automate the build process and generate the `kernel8.img` file. Follow the steps below to successfully compile the project:

## Prerequisites

1. **GCC Toolchain**: The Makefile uses the `arm-gnu-toolchain`, specifically the AArch64 cross-compiler. Ensure that this toolchain is installed.
   - You can download the toolchain from the official [Arm website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads).

2. **Path Adjustment**: The Makefile sets the path to the GCC toolchain via the `GCCPATH` variable. You need to adjust this path according to your installation.

   ```makefile
   GCCPATH = ../arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf/bin
   ```
   Replace the path with the correct path to your toolchain.

## Build Command

To build the project, run the following command in the directory containing the Makefile:

```bash
make
```

## Cleaning

To remove generated files (e.g., `kernel8.elf`, `.o`, and `.img` files), run:

```bash
make clean
```

