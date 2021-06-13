    .org $8000

init:
    lda #01
    sta $1000

    lda #$FF
    sta $1001

loop:
    inc $1000
    lda $1000

    dec $1001
    lda $1001

    jmp loop

reset_vector:
    .org $fffc
    .word $8000
    .word $0000
