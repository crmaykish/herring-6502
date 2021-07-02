    .include herring.s
    
    .org ROM_START

    ldx #%01010101

main_loop:
    inx
    jmp main_loop

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000
