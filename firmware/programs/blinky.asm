    PORTB = $C400
    DDRB = $C402

    .segment "CODE"

init:
    lda #$FF
    sta DDRB

    lda #$55

loop:
    sta PORTB
    rol
    jmp loop