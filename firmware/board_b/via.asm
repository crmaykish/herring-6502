.export _VIA_Init, _VIA_Write

PORTB = $C000
PORTA = $C001
DDRB = $C002
DDRA = $C003

.segment "CODE"

_VIA_Init:
    lda #$FF
    sta DDRA
    sta DDRB
    rts

; Write A to port A
_VIA_Write:
    sta PORTA
    rts