    .org $8000

    ldx %01010101

main_loop:
    inx
    jmp main_loop

    .org $fffc
    .word $8000
    .word $0000