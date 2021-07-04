    .include "herring.asm"

init:
    ; Reset ACIA
    lda #0
    sta ACIA_STATUS

    lda #$0B
    sta ACIA_COMMAND

    ; Set baudrate to 9600
    lda #$1E
    sta ACIA_CONTROL

restart:
    ldx #32

loop:
    clc
    cpx #126
    bcs restart

    lda ACIA_STATUS
    and #$10    ; mask off transmit buffer status flag
    beq loop

    txa
    sta ACIA_DATA

    inx

    jmp loop
    