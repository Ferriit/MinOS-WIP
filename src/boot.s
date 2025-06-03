; boot.s
[bits 32]
[extern kernel_main]

section .multiboot
    align 4
    dd 0x1BADB002            ; magic number
    dd 0                     ; flags
    dd -(0x1BADB002)         ; checksum

section .text
global _start
_start:
    cli
    call kernel_main
.hang:
    hlt
    jmp .hang
