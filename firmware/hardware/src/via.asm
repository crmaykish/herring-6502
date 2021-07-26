.include "herring.inc"

.export _VIA_OutputAll, _VIA_SetPortA, _VIA_SetPortB

.segment "CODE"

; Set all pins on all VIAs to output
_VIA_OutputAll:
    lda #VIA_OUTPUTS
    sta VIA0_DDRA
    sta VIA0_DDRB
    sta VIA1_DDRA
    sta VIA1_DDRB
    rts

; Write the A register to Port A of the VIA
_VIA_SetPortA:
    sta VIA0_PORTA
    rts

; Write the A register to port B of the VIA
_VIA_SetPortB:
    sta VIA0_PORTB
    rts
