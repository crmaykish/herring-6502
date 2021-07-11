; ### CONSTANTS ###
    ACIA_DATA = $DC00
    ACIA_STATUS = $DC01
    ACIA_COMMAND = $DC02
    ACIA_CONTROL = $DC03

    SERIAL_BUFFER_SIZE = 40

    CARRIAGE_RETURN = $0D
    LINE_FEED = $0A

; ### VARIABLES
    .segment "ZEROPAGE"
    serial_buffer: .res SERIAL_BUFFER_SIZE
    serial_index: .res 1

; ### SUBROUTINES ###
    .code

; start the ACIA at 9600 baud
acia_init:
    lda #$00
    sta ACIA_STATUS
    lda #$0B
    sta ACIA_COMMAND
    lda #$1E
    sta ACIA_CONTROL
    lda #0
    sta VIA_PORTA
    sta VIA_PORTB
    rts

; If ACIA Tx is ready, A is non-zero
acia_poll_tx:
    lda ACIA_STATUS
    and #$10
    rts

; Write A to the serial port (blocking)
acia_write_blocking:
    pha
not_ready:
    jsr acia_poll_tx
    beq not_ready
    pla
    sta ACIA_DATA
    rts

; Write the buffer starting at A until '\n' or X bytes
acia_write_buffer:
    ldx #0
acia_write_loop:
    lda serial_buffer,x
    jsr acia_write_blocking
    inx
    cpx #SERIAL_BUFFER_SIZE
    bcc acia_write_loop
    rts

; If ACIA Rx is ready, A is non-zero
acia_poll_rx:
    lda ACIA_STATUS
    and #$08
    rts

; Read from the serial port until a byte is returned (blocking)
acia_read_blocking:
    jsr acia_poll_rx
    beq acia_read_blocking
    lda ACIA_DATA
    rts

; Read one byte from the serial port into A (non-blocking, make sure there's data there first)
acia_read_nonblocking:
    lda ACIA_DATA
    rts

acia_write_newline:
    pha
    lda #LINE_FEED
    jsr acia_write_blocking
    lda #CARRIAGE_RETURN
    jsr acia_write_blocking
    pla
    rts
