ACIA = $F800

acia_init:
    ; reset
    lda #$00
    sta $F801

    ; RTS low, DTR low, interrupts disabled
    lda #$0B
    sta $F802

    ; Set baudrate to 19200
    lda #$1F
    sta $F803

restart:
    ldx #32

loop:
    stx $F800
    cpx #126
    beq restart
    inx
    jmp loop
