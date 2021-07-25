.include "herring.inc"

.export _VIA_OutputAll

.segment "CODE"

; Set all pins on all VIAs to output
_VIA_OutputAll:
    lda VIA_OUTPUTS
    sta VIA0_DDRA
    sta VIA0_DDRB
    sta VIA1_DDRA
    sta VIA1_DDRB
    rts
