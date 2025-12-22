/* =========================================================
 * linker/virt.ld
 * QEMU: qemu-system-arm -M virt -cpu cortex-a15
 * Bare-metal linker script (simple + scheduler-ready)
 * ========================================================= */

ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 128M
}

/* Tune this later when you add IRQ + scheduler stacks */
STACK_SIZE = 0x4000; /* 16 KB */

SECTIONS
{
  /* Start address */
  . = ORIGIN(RAM);

  /* Vector table / early boot (if you add one later) */
  .text.boot :
  {
    KEEP(*(.text.boot))
    KEEP(*(.vectors))
  } > RAM

  /* Code */
  .text :
  {
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame*)
  } > RAM

  /* Initialized data (VMA in RAM, LMA in RAM for -kernel ELF usage) */
  .data : ALIGN(8)
  {
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
  } > RAM

  /* Uninitialized data */
  .bss (NOLOAD) : ALIGN(8)
  {
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > RAM

  /* End of image */
  . = ALIGN(16);
  __end__ = .;

  /* Simple single stack region (top-down stack) */
  . = . + STACK_SIZE;
  . = ALIGN(16);
  __stack_top__ = .;

  /* Common aliases used by startup code */
  PROVIDE(_stack_top = __stack_top__);
  PROVIDE(_bss_start = __bss_start__);
  PROVIDE(_bss_end   = __bss_end__);
  PROVIDE(_end       = __end__);

  /* Discard anything we don’t want */
  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
  }
}
