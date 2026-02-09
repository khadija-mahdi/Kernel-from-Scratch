global gdt_flush

gdt_flush:
    MOV eax, [esp + 4]
    LGDT [eax]

    JMP 0x08:.flush     ; 0x08 = offset to kernel code segment (entry 1)
.flush:
    mov ax, 0x10        ; 0x10 = offset to kernel data segment (entry 2)
    mov ds, ax          ; Data Segment
    mov es, ax          ; Extra Segment
    mov fs, ax          ; FS Segment
    mov gs, ax          ; GS Segment

    mov ax, 0x18        ; 0x18 = offset to kernel stack segment (entry 3)
    mov ss, ax          ; Stack Segment
    RET

