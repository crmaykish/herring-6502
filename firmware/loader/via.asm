    VIA_PORTA = $C401
    VIA_PORTB = $C400
    VIA_DDRA = $C403
    VIA_DDRB = $C402

    .code

; Set all the pins to output
via_init:
    lda #$FF
    sta VIA_DDRA
    sta VIA_DDRB
    rts
