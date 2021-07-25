.export _ACIA_Init, _ACIA_Read, _ACIA_Write, _ACIA_NewLine

.include "herring.inc"

.segment "CODE"

; Set up the serial port
_ACIA_Init:
    pha

    lda #ACIA_RESET
    sta ACIA0_STATUS

    lda #ACIA_COMMAND_INIT
    sta ACIA0_COMMAND

    lda #ACIA_CONTROL_BAUD_19200
    sta ACIA0_CONTROL

    pla
    rts

; Read a byte from the serial port into A (blocks until a byte is available)
_ACIA_Read:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    beq _ACIA_Read
    sta ACIA0_DATA

; Write A to the serial port (blocks until the serial port receives the byte)
_ACIA_Write:
    pha
write_ready_check:
    lda ACIA0_STATUS
    and #ACIA_READY_TX
    beq write_ready_check
    pla
    sta ACIA0_DATA
    rts

; Write a new line to the serial port
_ACIA_NewLine:
    lda ASCII_NEWLINE
    jsr _ACIA_Write
    lda ASCII_RETURN
    jsr _ACIA_Write
    rts