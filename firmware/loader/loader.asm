    .setcpu "6502"

    .include "acia.asm"
    .include "via.asm"

    .segment "VECTORS"

    .word nmi
    .word reset
    .word irq

; ### CONSTANTS ###

; ### VARIABLES ###
    .segment "ZEROPAGE"
    
; ### CODE ###

    .code

reset:
    jmp main
nmi:
    rti
irq:
    rti

main:
    jsr via_init
    jsr acia_init

    ldx #0
    stx serial_index

main_loop:
    ; check for serial input
    jsr acia_poll_rx
    beq no_serial_in
    jsr store_serial
    jsr buffer_check

no_serial_in:
    stx VIA_PORTA
    inx

    jmp main_loop

store_serial:
    lda ACIA_DATA
    ldx serial_index
    sta serial_buffer,x
    inc serial_index
    rts

buffer_check:
    ldx serial_index
    cpx #SERIAL_BUFFER_SIZE
    bcs buf_full
    rts
buf_full:
    jsr acia_write_buffer
    jsr acia_write_newline
    ldx #0
    stx serial_index
    rts

    