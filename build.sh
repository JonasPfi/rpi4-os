#!/bin/bash

# Navigate to the kernel directory
cd kernel

# Check if the build directory exists; if not, create it
if [ ! -d "../build" ]; then
    mkdir ../build
fi

# Build the project
make

# Move the resulting kernel image to the build directory
mv build/kernel8.img ../build/

echo "Build completed successfully. Kernel image is located in the 'build' directory."
