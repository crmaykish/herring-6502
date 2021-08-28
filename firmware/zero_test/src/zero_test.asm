ACIA1_DATA = $8000
ACIA1_STATUS = $8001
ACIA1_COMMAND = $8002
ACIA1_CONTROL = $8003

VIA1_PORTB = $8400
VIA1_PORTA = $8401
VIA1_DDRB = $8402
VIA1_DDRA = $8403

.segment "CODE"

init:
    jsr via_init
    jsr acia_init

    ldy #0

restart:
    ldx #32
loop:
    sty VIA1_PORTA
    iny
    stx ACIA1_DATA
    stx VIA1_PORTB
    cpx #126
    beq restart
    inx
    
    jmp loop

acia_init:
    ; reset
    lda #$00
    sta ACIA1_STATUS

    ; RTS low, DTR low, interrupts disabled
    lda #$0B
    sta ACIA1_COMMAND

    ; Set baudrate to 19200
    lda #$1F
    sta ACIA1_CONTROL

    rts

via_init:
    lda #$FF
    sta VIA1_DDRB
    sta VIA1_DDRA

    lda #0
    sta VIA1_PORTB
    sta VIA1_PORTA

    rts