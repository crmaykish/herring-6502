.org $8000
lda #$00
loop:
sta $09
adc #$1
jmp loop
