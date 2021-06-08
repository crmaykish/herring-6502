    .org $8000

    lda #$01
    sta $1000

main_loop:
    inc $1000
    jmp main_loop

    .org $fffc
    .word $8000
    .word $0000