#!/usr/bin/env bash
# =========================================================
# Run Cortex-A bare-metal kernel on QEMU (virt machine)
# =========================================================

set -e

QEMU="qemu-system-arm"
KERNEL="build/kernel.elf"

if [ ! -f "$KERNEL" ]; then
    echo "ERROR: Kernel not found. Build first using 'make'."
    exit 1
fi

echo "Starting QEMU (Cortex-A15, virt)..."
echo "-------------------------------------"

exec $QEMU \
    -M virt \
    -cpu cortex-a15 \
    -nographic \
    -kernel "$KERNEL"
