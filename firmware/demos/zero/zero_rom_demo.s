RESET_VECTOR = $FFFC
ROM_START = $C000   ; Only the top half of the 32k ROM will be used

    .org ROM_START

init:
    lda #0

loop:
    inx
    jmp loop

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000