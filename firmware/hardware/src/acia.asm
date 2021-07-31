.export _acia_init, _putc, _getc, _print

.include "herring.inc"

.segment "ZEROPAGE"

ptr1: .res 2

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
    jsr wdc_bug         ; Comment out for non-WDC 65C51s
    rts

_getc:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    beq _getc
    lda ACIA0_DATA
    rts

_print:
    pha
    phy
    sta ptr1
    stx ptr1 + 1
    ldy #0
next_char:
    lda (ptr1),y
    beq end_of_string
    jsr _putc
    iny
    bne next_char
end_of_string:
    ply
    pla

    rts

; From: http://forum.6502.org/viewtopic.php?f=4&t=2543&start=30
wdc_bug:
    phy
    phx
delay_loop:
    ldy #2  ; Processor speed in MHz
minidelay:
    ldx #$68
delay_1:
    dex
    bne delay_1
    dey
    bne minidelay
    plx
    ply
    rts