; ACIA
ACIA_DATA = $6000
ACIA_STATUS = $6001
ACIA_COMMAND = $6002
ACIA_CONTROL = $6003

    .export _ACIA_Init
    .export _ACIA_Write

    .code

_ACIA_Init:
    pha

    ; Reset ACIA
    lda #0
    sta ACIA_STATUS

    lda #$0B
    sta ACIA_COMMAND

    ; Set baudrate to 9600
    lda #$1E
    sta ACIA_CONTROL

    pla
    rts

_ACIA_Write:
    pha
prv_wait_for_tx_rdy:
    lda ACIA_STATUS
    and #$10    ; mask off transmit buffer status flag
    beq prv_wait_for_tx_rdy

    pla
    sta ACIA_DATA

    rts