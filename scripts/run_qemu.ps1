# =========================================================
# Run Cortex-A bare-metal kernel on QEMU (virt machine)
# =========================================================

$QEMU = "qemu-system-arm"
$KERNEL = "build\kernel.elf"

if (-Not (Test-Path $KERNEL)) {
    Write-Host "ERROR: Kernel not found. Build first using 'make'."
    exit 1
}

Write-Host "Starting QEMU (Cortex-A15, virt)..."
Write-Host "-------------------------------------"

& $QEMU `
    -M virt `
    -cpu cortex-a15 `
    -nographic `
    -kernel $KERNEL
