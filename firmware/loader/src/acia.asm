.include "herring.inc"

.export _ACIA_Init, _ACIA_ReadByte, _ACIA_WriteByte

.segment "CODE"

_ACIA_Init:
    ; Reset the ACIA
    lda #ACIA_RESET
    sta ACIA0_STATUS

    ; Set the RTS/DTR and interrupt behavior
    lda #ACIA_COMMAND_INIT
    sta ACIA0_COMMAND

    ; Set the baudrate and data format
    lda #ACIA_CONTROL_BAUD_19200
    sta ACIA0_CONTROL

    rts

; Read a byte of data from the serial port (blocks until data is available)
; Serial byte is returned in A register
_ACIA_ReadByte:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    beq _ACIA_ReadByte
    lda ACIA0_DATA
    rts

; TODO: Writing bytes works at 65KHz, but not at 1MHz... why?

; Write the byte in register A to the serial port (blocks until serial port is ready to receive)
_ACIA_WriteByte:
    pha
    lda ACIA0_STATUS
    and #ACIA_READY_TX
    beq _ACIA_WriteByte
    pla
    sta ACIA0_DATA
    rts
