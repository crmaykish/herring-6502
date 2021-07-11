    VIA_PORTA = $C401
    VIA_PORTB = $C400
    VIA_DDRA = $C403
    VIA_DDRB = $C402

    .code

; Setup the VIA
VIA_Init:
    ; Set all the pins to outputs
    lda #$FF
    sta VIA_DDRA
    sta VIA_DDRB
    jsr VIA_Clear
    rts

VIA_Clear:
    pha
    ; Clear all the pins
    lda #0
    sta VIA_PORTA
    sta VIA_PORTB
    pla
    rts
