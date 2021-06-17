ROM_START = $8000
RESET_VECTOR = $FFFC

UART_DATA = $6000
UART_CMD = $6001
NEWLINE = $A

    .org ROM_START

    jmp init

restart:
    lda #NEWLINE            ; send a new line to the serial port
    jsr uart_print

init:
    ldx #0

loop:
    lda message,x
    beq restart
    jsr uart_print
    inx
    jmp loop

message: .asciiz "HERRING 6502!"

uart_print:
    sta UART_DATA
    lda #0
    sta UART_CMD
    lda #1
    sta UART_CMD
    rts

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000
