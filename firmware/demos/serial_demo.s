ROM_START = $8000
RESET_VECTOR = $FFFC

ACIA_DATA = $6000
ACIA_STATUS = $6001
ACIA_COMMAND = $6002
ACIA_CONTROL = $6003

    .org ROM_START

init:
    lda #%00001011      ; no parity, no echo, no interrupt
    sta ACIA_COMMAND
    lda #%00011111      ; 1 stop bit, 8 data bits, 19200 baud
    sta ACIA_CONTROL

write:
    ldx #0
next_char:
wait_txd_empty:
    lda ACIA_STATUS
    and #$10
    beq wait_txd_empty
    lda text,x
    beq read
    sta ACIA_DATA
    inx
    jmp next_char
read:
wait_rxd_full:
    lda ACIA_STATUS
    and #$08
    beq wait_rxd_full
    lda ACIA_DATA
    jmp write

text:
    .byte "Herring 6502", $0d, $0a, $00