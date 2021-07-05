    PORTB = $4000
    PORTA = $4001
    DDRB = $4002
    DDRA = $4003

    .export _VIA_Init
    .export _VIA_Write

    .code

_VIA_Init:
    pha

    lda #$FF
    sta DDRB
    lda #%01
    clc

    pla

    rts

_VIA_Write:

    sta PORTB

    rts