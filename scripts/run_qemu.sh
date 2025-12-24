#!/usr/bin/env bash
# =========================================================
# Run Cortex-A bare-metal kernel on QEMU (virt machine)
# =========================================================

set -e

VMLINUX=build/kernel.elf

echo "Starting QEMU (Cortex-A15, virt)..."
echo "-------------------------------------"

qemu-system-arm \
  -M virt \
  -cpu cortex-a15 \
  -nographic \
  -kernel build/kernel.elf \
  -chardev socket,id=uart0,host=127.0.0.1,port=5555,server=on,wait=on \
  -serial chardev:uart0 \
  -monitor none
