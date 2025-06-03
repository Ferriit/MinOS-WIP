section .bss
    seed resd 1          ; reserve 4 bytes for seed

section .text
    global srand
    global rand
    global time
    global wait_cycles:
    global measure_frequency
    global isr80_handler
    global idt_flush

    extern isr80_dispatch

; void srand(uint32_t new_seed)
; For 32-bit cdecl: argument at [esp + 4]

srand:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]   ; get new_seed argument
    mov [seed], eax
    pop ebp
    ret

; uint32_t rand(void)
rand:
    mov eax, [seed]
    mov ecx, 1103515245
    mul ecx              ; edx:eax = eax * ecx
    add eax, 12345
    mov [seed], eax
    ret

; uint32_t time(void)
; Returns lower 32 bits of CPU timestamp counter (RDTSC)
time:
    rdtsc                 ; EDX:EAX = 64-bit TSC value
    ; Return value in EAX (lower 32 bits)
    ret


wait_cycles:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
loop_label:
    dec eax
    jnz loop_label
    pop ebp
    ret
