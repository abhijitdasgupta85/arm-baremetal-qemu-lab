/* =========================================================
 * Cortex-A bare-metal startup code
 * QEMU virt, ARMv7-A (cortex-a15)
 * ========================================================= */

    .section .text.boot
    .global _start
    .type _start, %function

_start:
    /* Set up stack pointer */
    ldr sp, =_stack_top

    /* Clear BSS */
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0

bss_clear:
    cmp r0, r1
    strlt r2, [r0], #4
    blt bss_clear

    /* Call main() */
    bl main

halt:
    b halt
