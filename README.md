# rpi4-os

# Build Instructions

This project uses a sh to automate the build process and generate the `kernel8.img` file. Follow the steps below to successfully compile the project:


## Prerequisites

1. **GCC Toolchain**: The Makefile uses the `aarch64-elf` cross-compiler. Ensure that this toolchain is installed.
   - You can download the toolchain from the official [Arm website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads).

2. **ARMGNU Variable**: The Makefile defines the cross-compiler prefix with the `ARMGNU` variable. If your toolchain has a different prefix, adjust the `ARMGNU` variable in the Makefile:

   ```makefile
   ARMGNU ?= aarch64-elf
   ```

By following these steps and adjusting the Makefile variables and paths as necessary, you should be able to compile and build your project successfully.

## Build Command

To build the project, run the following command:

```bash
./build.sh
```


## UART Setup for Raspberry Pi 4

To enable UART communication on your Raspberry Pi 4, follow these steps to correctly connect the TTL-to-USB adapter to the Pi’s GPIO pins.

#### 1. Pin Connections:
Use the following GPIO pins on the Raspberry Pi 4 for UART:

- **TX (Transmit)**: GPIO 14 (Pin 8)
- **RX (Receive)**: GPIO 15 (Pin 10)
- **GND (Ground)**: Pin 6

Now, connect the TTL-to-USB adapter cables as follows:

- **Adapter TX** (usually an green wire) → **Raspberry Pi RX** (GPIO 15, Pin 10)
- **Adapter RX** (usually a white wire) → **Raspberry Pi TX** (GPIO 14, Pin 8)
- **Adapter GND** (usually a black wire) → **Raspberry Pi GND** (Pin 6)

#### 2. Enable UART on the Raspberry Pi 4:
To enable UART on the Pi, follow these steps:

1. Edit the config.txt file on the sd card:

2. Add the following lines to enable UART:

   ```bash
   enable_uart=1
   enable_gic=1
   hdmi_group=1
   hdmi_mode=16
   core_freq_min=500
   ```

#### 3. Use a Terminal Application:
Once connected, you can use a terminal application (like `Putty` on Windows or `minicom` on Linux) to interface with the Raspberry Pi via the UART connection. Ensure you set the baud rate to **115200**.

