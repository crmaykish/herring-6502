.export _acia_init, _acia_putc, _acia_getc, _print, _acia_rx_ready

.include "herring.inc"

.segment "ZEROPAGE"

ptr1: .res 2

.segment "CODE"

_acia_init:
    lda #ACIA_RESET
    sta ACIA0_STATUS
    lda #ACIA_COMMAND_INIT
    sta ACIA0_COMMAND
    lda #ACIA_CONTROL_BAUD_115200
    sta ACIA0_CONTROL
    rts

_acia_putc:
    pha
tx_check:
    lda ACIA0_STATUS
    and #ACIA_READY_TX
    beq tx_check
    pla
    sta ACIA0_DATA
    jsr wdc_bug
    rts

_acia_getc:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    beq _acia_getc
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
    jsr _acia_putc
    iny
    bne next_char
end_of_string:
    ply
    pla

    rts

_acia_rx_ready:
    lda ACIA0_STATUS
    and #ACIA_READY_RX
    rts

; WD65C51 Bug Workaround
;
; Note: this seems to work with WDC65C51s running at 4 MHz,
; but it does not work if the processor speed delay is actually set to 4 MHz
; CDP65C51 works with or without this code enabled
;
; See: http://forum.6502.org/viewtopic.php?f=4&t=2543&start=30
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