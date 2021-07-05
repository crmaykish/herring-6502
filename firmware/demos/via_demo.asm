    .setcpu "6502"
    
    PORTB = $4000
    PORTA = $4001
    DDRB = $4002
    DDRA = $4003

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

main:
init_via:
    ; Set all pins in port B to output
    lda #$FF
    sta DDRB

    lda #%01
    clc
loop:
    sta PORTB
    rol
    jmp loop
