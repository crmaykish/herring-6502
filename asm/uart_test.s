ROM_START = $8000
RESET_VECTOR = $FFFC

UART_DATA = $6000
UART_CMD = $6001
UART_CMD_SUBMIT = %01010101
NEWLINE = $A

    .org ROM_START

restart:
    lda #NEWLINE            ; send a new line to the serial port
    jsr uart_print
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
    lda UART_CMD_SUBMIT
    sta UART_CMD
    rts

    .org RESET_VECTOR
    .word ROM_START
    .word $0000
