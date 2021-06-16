ROM_START = $8000
RESET_VECTOR = $FFFC
PORTA = $4001
DDRA = $4003

    .org ROM_START

init:
    lda #%11111111          ; Set all of port A to output
    sta DDRA

    lda #%00000011

loop:
    sta PORTA
    rol
    jmp loop

    .org RESET_VECTOR
    .word ROM_START
    .word $0000
