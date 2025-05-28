ENTRY(_start)

SECTIONS
{
    . = 0x200000;  /* 2MB, standard for 64-bit kernels */

    .text : {
        *(.multiboot2)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    /* Add alignment for 64-bit */
    . = ALIGN(4096);
}