sei
lda #$FF
sta $C002
sta $C003
lda #0
sta $C000
sta $C001
loop:
nop
jmp loop
rts