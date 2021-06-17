ROM_START = $8000
RESET_VECTOR = $FFFC

UART_CMD_WRITE = $6000
UART_CMD_SUBMIT = $600F

NEW_LINE = $D

    .org ROM_START

loop:
    lda #$B
    sta UART_CMD_WRITE
    lda #0
    sta UART_CMD_SUBMIT

    lda #$E
    sta UART_CMD_WRITE
    lda #0
    sta UART_CMD_SUBMIT

    lda #$E
    sta UART_CMD_WRITE
    lda #0
    sta UART_CMD_SUBMIT

    lda #$F
    sta UART_CMD_WRITE
    lda #0
    sta UART_CMD_SUBMIT

    jmp loop

    .org RESET_VECTOR
    .word ROM_START
    .word $0000
