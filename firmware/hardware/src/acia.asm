.export _acia_init, _putc, _getc, _print

.include "herring.inc"

.segment "CODE"

_acia_init:
    lda #ACIA_RESET
    sta ACIA0_STATUS
    lda #ACIA_COMMAND_INIT
    sta ACIA0_COMMAND
    lda #ACIA_CONTROL_BAUD_19200
    sta ACIA0_CONTROL
    rts

_putc:
    pha
tx_check:
    lda ACIA0_STATUS
    and #ACIA_READY_TX
    beq tx_check
    pla
    sta ACIA0_DATA
    rts

_getc:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    beq _getc
    lda ACIA0_DATA
    rts

_print:
    ; TODO
    rts