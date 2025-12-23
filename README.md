**ARM Cortex-A Bare-Metal QEMU Lab**
------------------------------------------------------------------------
**Overview**
---------------------------------

This repository is a learning and experimentation lab for developing bare-metal software on ARM Cortex-A processors, using QEMU as the execution platform.

The project starts from first principles (boot, linker, UART bring-up) and incrementally evolves toward a scheduler-based runtime, similar in spirit to early RTOS or microkernel bring-up work.

The entire workflow is designed to be:
    - Reproducible
    - CI-friendly
    - GitHub Codespaces compatible
    - Close to real industry bring-up practices

**Key Objectives**
----------------------------------
- Bring up a bare-metal Cortex-A system on QEMU
- Implement UART TX/RX using PL011 (polling first, IRQ later)
- Understand startup, linker, and memory layout
- Build toward a cooperative → preemptive scheduler

Provide a clean base for:
  * context switching
  * timer interrupts
  * simple shell / console tasks

**Target Platform**

| Component      | Value                       |
| -------------- | --------------------------- |
| CPU            | ARM Cortex-A15              |
| Emulator       | QEMU `virt` machine         |
| UART           | PL011 @ `0x09000000`        |
| Execution Mode | Bare-metal (no OS, no libc) |
| Toolchain      | `arm-none-eabi-gcc`         |

**Repository Structure**
-----------------------------------------
    arm-baremetal-qemu-lab/
    ├── README.md
    ├── Makefile
    ├── linker/
    │   └── virt.ld
    ├── src/
    │   ├── start.S
    │   ├── main.c
    │   ├── uart_pl011.c
    │   └── uart_pl011.h
    ├── include/
    │   └── types.h
    ├── scripts/
    │   ├── run_qemu.sh
    │   └── run_qemu.ps1
    ├── tools/
    │   └── uart_sim.py
    └── .devcontainer/
        └── devcontainer.json

**Development Environment (Recommended)**
----------------------------------------------
This project is designed to work out of the box with GitHub Codespaces.

The repository includes a devcontainer.json that automatically installs:
 - ARM bare-metal GCC toolchain
 - QEMU (ARM system emulator)
 - GDB
 - Python (for UART simulation)

**Rebuild Codespace (one-time)**
- Open the repository in Codespaces
- Press Ctrl + Shift + P
- Select Codespaces: Rebuild Container

After this, no manual installation is required.

**Build Instructions**
-----------------------------------
From the repository root:
    make clean
    make

Artifacts:
    - build/kernel.elf — ELF image used by QEMU
    - (optional) build/kernel.lst — disassembly (via make dump)

**Running on QEMU**
------------------------------
**Linux/Codespaces/WSL**
    - ./scripts/run_qemu.sh

**Windows (PowerShell)**
   - .\scripts\run_qemu.ps1

**Expected output:**
   - UART Echo Test (QEMU Cortex-A)
   - Type characters to see them echoed back:

**To stop QEMU:**
    Ctrl + C

**UART Echo Demo**
----------------------------------------
The current main.c implements a UART echo loop:
    - Characters received on UART
    - Immediately transmitted back

This validates:
    - UART TX
    - UART RX
    - MMIO access
    - End-to-end host ↔ bare-metal interaction
    
**Host UART Simulator (Python)**
A simple Python script is provided to act as a UART simulator.
    python3 tools/uart_sim.py


This script:
    - Sends characters to the QEMU UART
    - Receives echoed data
    - Displays traffic on the host terminal
