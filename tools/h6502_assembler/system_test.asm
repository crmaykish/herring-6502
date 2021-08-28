.org $8000
lda #$00
sta $C002
sta $C003
loop:
sta $C000
adc #$1
jmp loop
