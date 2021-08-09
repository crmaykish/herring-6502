; .org $1000
; sei
; lda #$FF
; sta $C002
; sta $C003
; lda #0
; sta $C000
; sta $C001
; loop:
; nop
; jmp loop
; rts



; .org $1000
; jmp start
; brk
; start:
; rts

; .org $2000
; jmp shark
; brk
; brk
; nop
; shark:
; rts


.org $1000
ldx #$00
loop:
stx $C000
inx
jmp loop
