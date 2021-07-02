ROM_START = $8000
RESET_VECTOR = $FFFC

PORTB = $4000
PORTA = $4001
DDRB = $4002
DDRA = $4003

    .org ROM_START

init:
    ; Set all pins in port B to output
    lda #$FF
    sta DDRB

    lda #%00000101

loop:
    sta PORTB
    rol
    jmp loop

    .org RESET_VECTOR
    .word ROM_START
    .word $0000
