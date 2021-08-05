; Some demo code to test assembler

; init
sei

; VIA setup
lda #$FF
sta $C002
sta $C003
lda #0
sta $C000
sta $C001

rts
