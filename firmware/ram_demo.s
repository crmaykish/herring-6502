    .include herring.s

    .org ROM_START

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

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000
