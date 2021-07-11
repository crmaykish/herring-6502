; ### CONSTANTS ###
    ACIA_DATA = $DC00
    ACIA_STATUS = $DC01
    ACIA_COMMAND = $DC02
    ACIA_CONTROL = $DC03

    SERIAL_BUFFER_SIZE = 32

    CARRIAGE_RETURN = $0D
    LINE_FEED = $0A

; ### VARIABLES
    .segment "ZEROPAGE"
    serialBuffer: .res SERIAL_BUFFER_SIZE
    serialBufferIndex: .res 1

; ### SUBROUTINES ###
    .code

; ### PUBLIC API ###

; Setup the ACIA at 9600 baud
ACIA_Init:
    lda #$00
    sta ACIA_STATUS
    lda #$0B
    sta ACIA_COMMAND
    lda #$1E
    sta ACIA_CONTROL

    jsr ACIA_ClearBuffer

    rts

; Read a byte from the serial port into A (blocking)
ACIA_ReadByte:
    lda ACIA_STATUS
    and #$08
    beq ACIA_ReadByte
    lda ACIA_DATA
    rts

; Read into the serial buffer until a new line is seen or the max size is reached
; Echo each byte back to the serial port
; Note: does not store the carriage return character
ACIA_Readline:
    jsr ACIA_ReadByte
    jsr ACIA_WriteByte
    cmp #CARRIAGE_RETURN
    beq acia_readline_done
    jsr acia_store_byte
    jmp ACIA_Readline
acia_readline_done:
    rts

; Write A to the serial port (blocking)
ACIA_WriteByte:
    pha
acia_tx_check:
    lda ACIA_STATUS
    and #$10
    beq acia_tx_check
    pla
    sta ACIA_DATA
    rts

; Write the entire buffer to the serial port
ACIA_WriteBuffer:
    ldx #0
acia_write_loop:
    lda serialBuffer,x
    jsr ACIA_WriteByte
    inx
    cpx #SERIAL_BUFFER_SIZE
    bcc acia_write_loop
    rts

; Write the buffer as a string, ending with a newline
ACIA_PrintLine:
    ldx #0
acia_printline_loop:
    lda serialBuffer,x
    beq acia_printline_done
    jsr ACIA_WriteByte
    inx
    cpx #SERIAL_BUFFER_SIZE
    bcc acia_printline_loop
acia_printline_done:
    jsr ACIA_NewLine
    rts

; Write a newline to the serial port
ACIA_NewLine:
    pha
    lda #LINE_FEED
    jsr ACIA_WriteByte
    lda #CARRIAGE_RETURN
    jsr ACIA_WriteByte
    pla
    rts

; clear the serial buffer
ACIA_ClearBuffer:
    pha
    ldx #0
    lda #0
acia_clear_buffer_loop:
    sta serialBuffer,x
    inx
    cpx #SERIAL_BUFFER_SIZE
    bcs acia_clear_buffer_done
    jmp acia_clear_buffer_loop
acia_clear_buffer_done:
    ldx #0
    stx serialBufferIndex
    pla
    rts

; ### PRIVATE SUBROUTINES ###

; Store A into the serial buffer and increment the index
acia_store_byte:
    ldx serialBufferIndex
    sta serialBuffer,x
    inc serialBufferIndex
    rts
