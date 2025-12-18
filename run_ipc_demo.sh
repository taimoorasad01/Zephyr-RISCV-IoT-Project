#!/bin/bash
# Enable the Python environment
source ~/zephyr-project/.venv/bin/activate

# Setup Zephyr environment variables
source ~/zephyr-project/zephyr/zephyr-env.sh

# Build and Run the project
west build -b qemu_riscv32 ~/zephyr-project/my_ipc_app
west build -t run
