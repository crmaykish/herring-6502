    .setcpu "6502"

    .include "herring.asm"

    .segment "VECTORS"

    .word nmi
    .word reset
    .word irq

    .data

    .code

reset:
    jmp main

nmi:
    rti

irq:
    rti

; ### MAIN ###

main:
    jsr via_init
    jsr acia_init

    ldx #0
restart_serial:
    ldy #32

main_loop:
    cpy #128
    bcs restart_serial
    tya
    jsr acia_write_blocking
    txa
    jsr via_write_a
    jsr via_write_b
    inx
    iny

    jmp main_loop

; ### ACIA ###

; start the ACIA at 9600 baud
acia_init:
    lda #$00
    sta ACIA_STATUS
    lda #$0B
    sta ACIA_COMMAND
    lda #$1E
    sta ACIA_CONTROL
    rts

; Write A to the serial port (blocking)
acia_write_blocking:
    pha
acia_ready_tx:
    lda ACIA_STATUS
    and #$10
    beq acia_ready_tx
    pla
    sta ACIA_DATA
    rts

; If ACIA Rx is ready, A is non-zero
acia_ready_rx:
    lda ACIA_STATUS
    and #$08
    rts

; Read one byte from the serial port into A (blocking)
acia_read_blocking:
    jsr acia_ready_rx
    beq acia_read_blocking
    lda ACIA_DATA
    rts

; ### VIA ###

via_init:
    lda #$FF
    sta VIA_DDRA
    sta VIA_DDRB
    rts

via_write_a:
    sta VIA_PORTA
    rts

via_write_b:
    sta VIA_PORTB
    rts
