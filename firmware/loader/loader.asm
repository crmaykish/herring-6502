    .setcpu "6502"

    .segment "VECTORS"

    .word nmi
    .word reset
    .word irq

    .code

reset:
    jmp main
nmi:
    rti
irq:
    rti

main:
    sei
    jsr VIA_Init
    jsr ACIA_Init

main_loop:
    jsr LOADER_ShowPrompt
    jsr ACIA_Readline
    jsr ACIA_NewLine
    jsr ACIA_PrintLine

    ; TODO: Do something with the input command

    jsr ACIA_ClearBuffer

    jmp main_loop

LOADER_ShowPrompt:
    lda PROMPT
    jsr ACIA_WriteByte
    lda PROMPT+1
    jsr ACIA_WriteByte

; ### INCLUDES ### (includes at the bottom keep the reset vector consistent)
    .include "acia.asm"
    .include "via.asm"

; ### CONSTANTS ###
; WELCOME: .byte "Herring 6502 ><(((°>", 0
PROMPT: .byte "> ", 0
