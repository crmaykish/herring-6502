    .setcpu "6502"

    .export __STARTUP__ : absolute = 1

    .import __RAM_START__
    .import __RAM_SIZE__
    .import _main
    .import zerobss
    .import copydata

    .include "zeropage.inc"

    .segment "VECTORS"

    .word   nmi
    .word   reset
    .word   irq

    .segment  "STARTUP"

reset:      jmp init

nmi:        rti

irq:        rti

init:       cld
    ldx #$ff
    txs

    lda #<(__RAM_START__ + __RAM_SIZE__)
    sta sp
    lda #>(__RAM_START__ + __RAM_SIZE__)
    sta sp + 1

    jsr zerobss
    jsr copydata

    jsr _main