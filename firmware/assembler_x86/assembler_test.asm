; Some demo code to test assembler

; VIA setup
lda #$FF
sta $C002
sta $C003
lda #0
sta $C000
sta $C001

sei
clc

jmp $1000

rts