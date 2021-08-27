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

via_init:
    lda #$00
    sta $F402
    sta $F403
    sta $F400
    sta $F401

restart:
    ldx #32

loop:
    stx $F800
    stx $F400
    stx $F401
    cpx #126
    beq restart
    inx
    jmp loop
