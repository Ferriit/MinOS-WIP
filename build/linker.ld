ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot :
    {
        KEEP(*(.multiboot))  /* ensure it's not discarded */
    }
    .text : {
        *(.multiboot)
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
}
